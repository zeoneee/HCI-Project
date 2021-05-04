#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

Mat ROI(Mat img, Mat back, Mat img2_fg) {
	// �̹����� ����, �ʺ�, ä�� �� ���ϱ� 
	int width = img.cols;				// ũ�� ���� ���� 
	int height = img.rows;

	int backWidth = back.cols;		// original frame 
	int backHeight = back.rows;


	// ��� �̹����� Region Of Interest ���ϱ�
	Mat roi = back(Rect(backWidth / 2 - width / 2, backHeight / 2 - height / 2, width, height));

	addWeighted(roi, 1, img2_fg, 0.5, 0, roi);

	imshow("result", back);

	return back;
}