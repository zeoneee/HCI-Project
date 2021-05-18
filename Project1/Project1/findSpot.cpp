#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

Mat ROI(Mat img, Mat back, Mat img2_fg, Point p) {
	// 이미지의 높이, 너비, 채널 수 구하기 
	int width = img.cols;				// 크기 줄인 도안 
	int height = img.rows;
	
	int backWidth = p.x;		// original frame 
	int backHeight = p.y;

	// 배경 이미지의 Region Of Interest 구하기
	if (0 < backWidth
		&& 0 < width
		&& backWidth + width / 2 <= back.cols
		&& 0 < backHeight
		&& 0 < height
		&& backHeight + height / 2 <= back.rows) { // 피부가 인식 되었을 때 
		Mat roi = back(Rect(backWidth - width / 2, backHeight - height / 2, width, height));

		// 배경 이미지의 ROI영역과 워터마크 이미지의 워터마크 영역을 블렌딩
		addWeighted(roi, 1, img2_fg, 0.5, 0, roi);

		imshow("result", back);
	}
	else {	// 피부가 인식 되지 않았을 때 
		imshow("result", back);
	}

	return back;
}