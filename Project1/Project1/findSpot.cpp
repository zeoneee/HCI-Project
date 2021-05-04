#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

Mat ROI(Mat img, Mat back, Mat img2_fg) {
	// 이미지의 높이, 너비, 채널 수 구하기 
	int width = img.cols;				// 크기 줄인 도안 
	int height = img.rows;

	int backWidth = back.cols;		// original frame 
	int backHeight = back.rows;


	// 배경 이미지의 Region Of Interest 구하기
	Mat roi = back(Rect(backWidth / 2 - width / 2, backHeight / 2 - height / 2, width, height));

	addWeighted(roi, 1, img2_fg, 0.5, 0, roi);

	imshow("result", back);

	return back;
}