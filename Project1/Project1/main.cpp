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
	string image = chooseImage();		// ImageProcess.cpp 에서 구현
	cout << image << endl;
	Mat blendImg = imread(image, 1);
	//imshow("blending",blendImg);

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

		// 4번 피부 부분 추출 
		Mat Skin_Area;
		inRange(YCrCb, Scalar(0, 130, 90), Scalar(255, 168, 130), Skin_Area);
		imshow("Skin_Area", Skin_Area);

		// 중심점 찾기 
		Moments m = moments(Skin_Area, true);
		Point p(m.m10 / m.m00, m.m01 / m.m00);
		cout << p.x <<"  "<<p.y<< endl;


		// 5번 피부 부분 영상만 잘라내기  
		cvtColor(Skin_Area, Skin_Area, CV_GRAY2BGR);
		Mat image_copy = Mask + (~Skin_Area);
		imshow("temp", image_copy);
		

		//https://velog.io/@oosiz/OpenCV-Python-%EB%8B%A4%EB%A5%B8-%ED%81%AC%EA%B8%B0-%EC%9D%B4%EB%AF%B8%EC%A7%80-%EB%B8%94%EB%A0%8C%EB%94%A9
		// 합성할 이미지에서 캐릭터 부분만 추출
		
		Mat img_small;
		resize(blendImg, img_small, Size(100, 100));	// 이미지 크기 줄이기

		Mat img2_fg = extract(img_small);	// 크기 줄인 도안과 그것의 워터마크 부분만 추출한 도안 반환 


		// 배경 이미지의 Region Of Interest 구하기
		Mat roi = ROI(img_small, image_copy, img2_fg, p);

		// 배경 이미지의 ROI영역과 워터마크 이미지의 워터마크 영역을 블렌딩
		



		


		char c = (char)waitKey(10);
		if (c == 27) break;
	}
	return 0;
}

