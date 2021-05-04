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
	// 비디오 이미지 불러오기 
	VideoCapture cam(0);
	// 원래의 오리지널 이미지 
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

		// 마스크를 이용한 전처리
		resize(HAND_MASK, HAND_MASK, Size(Origin_frame.cols, Origin_frame.rows), 0, 0, CV_INTER_LINEAR);

		Mat Mask = Origin_frame + HAND_MASK;

		// 3번 컬러 모델 적용 
		Mat YCrCb;
		cvtColor(Mask, YCrCb, CV_BGR2YCrCb);

		//// 4번 피부 부분 추출 
		Mat Skin_Area;
		inRange(YCrCb, Scalar(0, 130, 90), Scalar(255, 168, 130), Skin_Area);
		imshow("Skin_Area", Skin_Area);

		// 윤곽선 검색  https://learnopencv.com/contour-detection-using-opencv-python-c/
		std::vector<std::vector<Point>> contours;  // 각 도형의 윤곽선을 표현하는 포인트 요소를 벡터로 표현해서 가진다/
		std::vector<Vec4i> hierarchy;	// 4개 정수형 데이터 요소로 이미지 토폴로지 정보 저장
		findContours(Skin_Area, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

		cvtColor(Skin_Area, Skin_Area, CV_GRAY2BGR);
		Mat image_copy = Mask + (~Skin_Area);
		imshow("temp", image_copy);

		drawContours(image_copy, contours, -1, Scalar(0, 255, 0), 2);

		//imshow("Simple approximation", image_copy);
		
		

		// 손의 Convex Hull 구하기 https://learnopencv.com/convex-hull-using-opencv-in-python-and-c/
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

		// 결함 찾기 https://github.com/3rang/Fingertip_Detection/blob/main/FingerTipDetect_v1.cpp
	
		char c = (char)waitKey(10);
		if (c == 27) break;
	}
	return 0;
}

