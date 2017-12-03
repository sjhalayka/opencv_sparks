#include <opencv2/opencv.hpp>
using namespace cv;
#pragma comment(lib, "opencv_world331.lib")

#include <iostream>
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

	for (int i = 0; i < frame.cols; i++)
	{
		size_t spark_count = 0;

		bool lit = false;

		if (255 == frame.at<unsigned char>(0, i))
		{
			lit = true;
			spark_count++;
		}

		for (int j = 1; j < frame.rows; j++)
		{
			if (255 == frame.at<unsigned char>(j, i) && lit == false)
			{
				lit = true;
				spark_count++;
			}
			else if (0 == frame.at<unsigned char>(j, i) && lit == true)
			{
				lit = false;
			}
		}

		cout << spark_count << endl;
	}

	imshow("frame", frame);

	waitKey();

	return 0;
}