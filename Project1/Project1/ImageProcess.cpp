#include <iostream>
#include "ImageProcess.h"
using namespace std;

string chooseImage() {
	int k;
	string res;

	Mat example = imread("image/all.png");
	imshow("Example", example);

	cout << "원하시는 이미지의 번호를 입력해주세요. (그 외 '0' 입력)" << endl;
	cin >> k;
	switch (k) {
	case 0:
		cout << "파일 경로를 입력해 주세요" << endl;
		cin >> res;
	case 1:
		res = "image/1.jpg";
		break;
	case 2:
		res = "image/2.jpg";
		break;
	case 3:
		res = "image/3.jpg";
		break;
	case 4:
		res = "image/4.jpg";
		break;
	case 5:
		res = "image/5.jpg";
		break;
	case 6:
		res = "image/6.jpg";
		break;
	case 7:
		res = "image/7.jpg";
		break;
	case 8:
		res = "image/8.jpg";
		break;
	case 9:
		res = "image/9.jpg";
		break;
	case 10:
		res = "image/10.jpg";
		break;
	default:
		cout << "잘못된 입력입니다." << endl;
	}
	return res;
}

Mat extract(Mat img) {

	Mat img2gray; // 합성할 이미지를 흑백으로 변환 
	cvtColor(img, img2gray, CV_BGR2GRAY);

	Mat mask;	// 흑백 이미지를 임계값으로 이진화해 마스크 생성
	threshold(img2gray, mask, 250, 255, THRESH_BINARY);

	// 해당 마스크의 역처리
	Mat mask_inv;
	bitwise_not(mask, mask_inv);
	imshow("mask_inv", mask_inv);

	// 워터마크 이미지에서 워터마크 영역만 추출(and 비트연산, 역 마스크 영역)
	Mat img2_fg;
	bitwise_and(img, img, img2_fg, mask = mask_inv);
	imshow("img_small", img);
	
	return img2_fg;
}
