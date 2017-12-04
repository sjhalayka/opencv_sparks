#include <opencv2/opencv.hpp>
using namespace cv;
#pragma comment(lib, "opencv_world331.lib")

#include <iostream>
#include <vector>
using namespace std;


int main(void)
{
	Mat frame = imread("sparks.png", CV_LOAD_IMAGE_GRAYSCALE);

	if (frame.empty())
	{
		cout << "Error loading image file" << endl;
		return -1;
	}

	threshold(frame, frame, 127, 255, THRESH_BINARY);

	imshow("thresh", frame);

	// Start with the second column
	for (int i = 1; i < frame.cols; i++)
	{
		size_t spark_count = 0;
		size_t bg_count = 0;
		bool lit = false;
		vector<int> begin_end_black_regions;

		// Start with the first row
		if (255 == frame.at<unsigned char>(0, i))
		{
			lit = true;
			spark_count++;
		}
		else
		{
			lit = false;
			bg_count++;
			begin_end_black_regions.push_back(0);
		}

		// Start with the second row
		for (int j = 1; j < frame.rows - 1; j++)
		{
			if (255 == frame.at<unsigned char>(j, i) && lit == false)
			{
				lit = true;
				spark_count++;
				begin_end_black_regions.push_back(j - 1);
			}
			else if (0 == frame.at<unsigned char>(j, i) && lit == true)
			{
				lit = false;
				bg_count++;
				begin_end_black_regions.push_back(j);
			}
		}

		// End with the last row
		if (0 == frame.at<unsigned char>(frame.rows - 1, i))
		{
			begin_end_black_regions.push_back(frame.rows - 1);
		}
		else if(255 == frame.at<unsigned char>(frame.rows - 1, i) && lit == false)
		{
			begin_end_black_regions.push_back(frame.rows - 2);
		}

		for (int k = 0; k < begin_end_black_regions.size(); k += 2)
		{
			bool found_branch = true;

			for (int l = begin_end_black_regions[k]; l <= begin_end_black_regions[k + 1]; l++)
			{
				if (0 == frame.at<unsigned char>(l, i - 1))
				{
					found_branch = false;
					break;
				}
			}

			if (found_branch == true)
			{
				Point2i location(i - 1, begin_end_black_regions[k]);

				circle(frame, location, 2, Scalar(127, 127, 127), 2);
			}
		}
	}

	imshow("frame", frame);

	waitKey();

	return 0;
}