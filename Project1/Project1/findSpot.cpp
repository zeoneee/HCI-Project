#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

Mat ROI(Mat img, Mat back, Mat img2_fg, Point p) {
	// �̹����� ����, �ʺ�, ä�� �� ���ϱ� 
	int width = img.cols;				// ũ�� ���� ���� 
	int height = img.rows;
	
	int backWidth = p.x;		// original frame 
	int backHeight = p.y;

	// ��� �̹����� Region Of Interest ���ϱ�
	if (0 < backWidth
		&& 0 < width
		&& backWidth + width / 2 <= back.cols
		&& 0 < backHeight
		&& 0 < height
		&& backHeight + height / 2 <= back.rows) { // �Ǻΰ� �ν� �Ǿ��� �� 
		Mat roi = back(Rect(backWidth - width / 2, backHeight - height / 2, width, height));

		// ��� �̹����� ROI������ ���͸�ũ �̹����� ���͸�ũ ������ ����
		addWeighted(roi, 1, img2_fg, 0.5, 0, roi);

		imshow("result", back);
	}
	else {	// �Ǻΰ� �ν� ���� �ʾ��� �� 
		imshow("result", back);
	}

	return back;
}