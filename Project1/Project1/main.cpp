#include <opencv2/opencv.hpp>
#include <iostream>
#include "ImageProcess.h"
#include "findSpot.h"
#define FilePath "image/1.png"


#define CAM_WIDTH 600
#define CAM_HEIGHT 400

using namespace cv;
using namespace std;

int main() {
	string image = chooseImage();		// ImageProcess.cpp ���� ����
	cout << image << endl;
	Mat blendImg = imread(image, 1);
	//imshow("blending",blendImg);

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

		// 4�� �Ǻ� �κ� ���� 
		Mat Skin_Area;
		inRange(YCrCb, Scalar(0, 130, 90), Scalar(255, 168, 130), Skin_Area);
		imshow("Skin_Area", Skin_Area);

		// 5�� �Ǻ� �κ� ���� �߶󳻱�  
		cvtColor(Skin_Area, Skin_Area, CV_GRAY2BGR);
		Mat image_copy = Mask + (~Skin_Area);
		imshow("temp", image_copy);
		

		//https://velog.io/@oosiz/OpenCV-Python-%EB%8B%A4%EB%A5%B8-%ED%81%AC%EA%B8%B0-%EC%9D%B4%EB%AF%B8%EC%A7%80-%EB%B8%94%EB%A0%8C%EB%94%A9
		// �ռ��� �̹������� ĳ���� �κи� ����
		
		Mat img_small;
		resize(blendImg, img_small, Size(100, 100));	// �̹��� ũ�� ���̱�

		Mat img2_fg = extract(img_small);	// ũ�� ���� ���Ȱ� �װ��� ���͸�ũ �κи� ������ ���� ��ȯ 


		// ��� �̹����� Region Of Interest ���ϱ�
		Mat roi = ROI(img_small, image_copy, img2_fg);

		// ��� �̹����� ROI������ ���͸�ũ �̹����� ���͸�ũ ������ ����
		



		


		char c = (char)waitKey(10);
		if (c == 27) break;
	}
	return 0;
}

