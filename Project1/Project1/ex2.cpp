
#include <iostream>
#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

unsigned char Camera_id = 0;

int largeid = 0;
int maxArea = 0;

#define H_MIN  0// minimum Hue
#define H_MAX  255 // maximum Hue
#define S_MIN  35 // minimum Saturation
#define S_MAX  255 // maximum Saturation
#define V_MIN  97 // minimum Value
#define V_MAX  255 //maximum Value

#define offSetP1 7
#define offSetP2 10
#define offSetP3 20



class OpencvMainEngineInit
{

public:

	Mat Grab2Binary(Mat src, Mat out, Mat out_threshold)
	{
		cvtColor(src, out, cv::COLOR_BGR2HSV);

		GaussianBlur(out, out, Size(5, 5), 0);
		//thresholding 
		inRange(out, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), out_threshold);

		return out_threshold;

	}

	Mat frameThroesholding(Mat input)
	{
		//morphological transformations

	//	erode(input, input, getStructuringElement(MORPH_RECT, Size(1, 1)));

		dilate(input, input, getStructuringElement(MORPH_RECT, Size(5, 5)));

		return input;
	}



};


/***********************************************************************
 *Function Name :- main
 *
 *Descrption    :- Program start point and finding .wav file in given location and make thread to convert into .mp3
 *
 *Argument      :- command line arguments (bin with folder path)
 *
 *Return Type   :- int
 ************************************************************************/



int main(int argc, char** argv)
{

	Mat rgbFrame, grayFrame, filterFrame;

	// open the default camera



	// initilization of Process.

	OpencvMainEngineInit MainEngine;

	cout << "Please enter the camera interface value (for extern camera interface)" << endl;

	if (argc != 2)
	{
		cout << "We have selected the In-build camera interface " << endl;


	}
	else
	{
		Camera_id = atoi(argv[1]);
		cout << "Updated  camera interface " << endl;

	}

	//Capturing live frame from camera

	VideoCapture inStream(Camera_id);

	if (!inStream.isOpened())
	{
		cout << "Can't able to open the camera interface" << endl;
		exit;

	}


	while (true)
	{
		// Reading the default camera

		inStream.read(rgbFrame);

		filterFrame = MainEngine.Grab2Binary(rgbFrame, grayFrame, filterFrame); //To get Binary Image

		filterFrame = MainEngine.frameThroesholding(filterFrame); // Making fine image using some thresholding.

		vector<vector<Point>> Contours;

		vector<Vec4i> hier;


		//finding the contour of largest area and storing its index

		findContours(filterFrame, Contours, hier, RETR_CCOMP, CHAIN_APPROX_NONE, Point(0, 0));

		largeid = 0;
		maxArea = 0;

		for (int i = 0; i < Contours.size(); i++)
		{
			double a = contourArea(Contours[i]);
			if (a > maxArea)
			{
				maxArea = a;
				largeid = i;
			}
		}

		vector<vector<int> > hullHand(Contours.size());

		vector<vector<Vec4i >> defects(Contours.size());

		for (int i = 0; i < Contours.size(); i++)
		{

			convexHull(Contours[i], hullHand[i], false);
			if (hullHand[i].size() > 3)
			{
				convexityDefects(Contours[i], hullHand[i], defects[i]);
			}
		}
		if (maxArea > 5000) {

			//  convexityDefects
			for (int j = 0; j < defects[largeid].size(); ++j)
			{
				const Vec4i& v = defects[largeid][j];
				float depth = v[3] / 256;
				if (depth > 20) //  filter defects by depth
				{
					int startidx = v[0];
					Point ptStart(Contours[largeid][startidx]);  // point of the contour where the defect begins

					Point pt1(ptStart.x - offSetP1, ptStart.y);
					Point pt2(ptStart.x + offSetP2, ptStart.y + offSetP3);

					//draw rectangle for the biggest contour
					rectangle(rgbFrame, pt1, pt2, Scalar(0, 255, 0));

				}
			}
		}

		imshow("Raw_Frame", filterFrame);

		imshow("FingerTip", rgbFrame);

		if (cv::waitKey(1) >= 0) break;

		Contours.clear();
		hier.clear();

	}

	inStream.release();

	cv::destroyAllWindows();
}
