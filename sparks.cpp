#include <opencv2/opencv.hpp>
using namespace cv;
#pragma comment(lib, "opencv_world331.lib")

#include <iostream>
#include <vector>
using namespace std;


int main(void)
{
	Mat frame = imread("sparks.png");

	if (frame.empty())
	{
		cout << "Error loading image file" << endl;
		return -1;
	}

	Mat colour_frame = frame.clone();

	cvtColor(frame, frame, CV_BGR2GRAY);

	threshold(frame, frame, 127, 255, THRESH_BINARY);

	vector<Point2i> branch_locations;

	// Start with the second column
	for (int i = 1; i < frame.cols; i++)
	{
		bool lit = false;
		vector<int> begin_black_regions;
		vector<int> end_black_regions;

		// Start with the first row
		if (255 == frame.at<unsigned char>(0, i))
		{
			lit = true;
		}
		else
		{
			lit = false;
			begin_black_regions.push_back(0);
		}

		// Start with the second row
		for (int j = 1; j < frame.rows - 1; j++)
		{
			if (255 == frame.at<unsigned char>(j, i) && lit == false)
			{
				lit = true;
				end_black_regions.push_back(j - 1);
			}
			else if (0 == frame.at<unsigned char>(j, i) && lit == true)
			{
				lit = false;
				begin_black_regions.push_back(j);
			}
		}

		// End with the last row
		if (0 == frame.at<unsigned char>(frame.rows - 1, i) && lit == false)
		{
			end_black_regions.push_back(frame.rows - 1);
		}
		else if (0 == frame.at<unsigned char>(frame.rows - 1, i) && lit == true)
		{
			begin_black_regions.push_back(frame.rows - 1);
			end_black_regions.push_back(frame.rows - 1);
		}
		else if (255 == frame.at<unsigned char>(frame.rows - 1, i) && lit == false)
		{
			end_black_regions.push_back(frame.rows - 2);
		}

		if(begin_black_regions.size() != end_black_regions.size())
			cout << begin_black_regions.size() << " " << end_black_regions.size() << endl;

		// for each black region begin/end pair
		for (size_t k = 0; k < begin_black_regions.size(); k++)
		{
			bool found_branch = true;

			for (int l = begin_black_regions[k]; l <= end_black_regions[k]; l++)
			{
				if (0 == frame.at<unsigned char>(l, i - 1))
				{
					found_branch = false;
					break;
				}
			}

			if (found_branch == true)
			{
				Point2i location(i - 1, begin_black_regions[k]);
				branch_locations.push_back(location);
			}
		}
	}

	for (size_t i = 0; i < branch_locations.size(); i++)
		circle(colour_frame, branch_locations[i], 2, Scalar(255, 127, 0), 2);

	imshow("frame", colour_frame);

	waitKey();

	return 0;
}