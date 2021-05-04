#include <opencv2/opencv.hpp>
#include "Edge.h"
#include "ImageAnalysis.h"
#include "Morph.h"
#include "Pixel.h"
#include "Util.h"
#include "Color.h"
#include "Geometry.h"
#include "Filter.h"
#define FilePath "image/1.png"
#define CAM_WIDTH 600
#define CAM_HEIGHT 400

using namespace cv;

int main() {
	// ���� �̹��� �ҷ����� 
	VideoCapture cam(0);
	// ������ �������� �̹��� 
	cam.set(CV_CAP_PROP_FRAME_WIDTH, CAM_WIDTH);
	cam.set(CV_CAP_PROP_FRAME_HEIGHT, CAM_HEIGHT);
	Mat Origin_frame;
	Mat HAND_MASK = imread(FilePath, 1);

	if (!cam.isOpened()) { printf("--(!)Error opening video cam\n"); return -1; }
	while (cam.read(Origin_frame)) {
		if (Origin_frame.empty()) {
			printf("--(!) No cam frame -- Break!");
			break;
		}

		// ����ũ�� �̿��� ��ó��
		resize(HAND_MASK, HAND_MASK, Size(Origin_frame.cols, Origin_frame.rows), 0, 0, CV_INTER_LINEAR);

		Mat Mask = Origin_frame + HAND_MASK;

		// 3�� �÷� �� ���� 
		Mat YCrCb;
		cvtColor(Mask, YCrCb, CV_BGR2YCrCb);

		//// 4�� �Ǻ� �κ� ���� 
		Mat Skin_Area;
		inRange(YCrCb, Scalar(0, 130, 90), Scalar(255, 168, 130), Skin_Area);
		imshow("Skin_Area", Skin_Area);

		// ������ �˻�  https://learnopencv.com/contour-detection-using-opencv-python-c/
		std::vector<std::vector<Point>> contours;  // �� ������ �������� ǥ���ϴ� ����Ʈ ��Ҹ� ���ͷ� ǥ���ؼ� ������/
		std::vector<Vec4i> hierarchy;	// 4�� ������ ������ ��ҷ� �̹��� �������� ���� ����
		findContours(Skin_Area, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

		cvtColor(Skin_Area, Skin_Area, CV_GRAY2BGR);
		Mat image_copy = Mask + (~Skin_Area);
		imshow("temp", image_copy);

		drawContours(image_copy, contours, -1, Scalar(0, 255, 0), 2);

		//imshow("Simple approximation", image_copy);
		
		

		// ���� Convex Hull ���ϱ� https://learnopencv.com/convex-hull-using-opencv-in-python-and-c/
		// create hull array from convex hull points

		

		std::vector<std::vector<Point>> hull(contours.size());
		for (int i = 0; i < contours.size(); i++) {
			convexHull(Mat(contours[i]), hull[i], false);
			
		}
		// create a blank image (black image)
		Mat drawing = Mat::zeros(Skin_Area.size(), CV_8UC3);

		for (int i = 0; i < contours.size(); i++) {
			Scalar color_contours = Scalar(0, 255, 0); // green - color for contours
			Scalar color = Scalar(255, 0, 0); // blue - color for convex hull
			// draw ith contour
			drawContours(drawing, contours, i, color_contours, 1, 8, std::vector<Vec4i>(), 0, Point());
			// draw ith convex hull
			drawContours(drawing, hull, i, color, 1, 8, std::vector<Vec4i>(), 0, Point());
		}
		imshow("drawing1", drawing);


		// https://abnerrjo.github.io/blog/2015/11/15/fingertip-detection-on-opencv/

		// ���� ã�� https://github.com/3rang/Fingertip_Detection/blob/main/FingerTipDetect_v1.cpp
	
		char c = (char)waitKey(10);
		if (c == 27) break;
	}
	return 0;
}

