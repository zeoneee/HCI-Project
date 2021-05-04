#include <iostream>
#include "ImageProcess.h"
using namespace std;

string chooseImage() {
	int k;
	string res;

	cout << "어떤 이미지를 이용하시겠습니까?" << endl;
	cout << "1.짱구  2.도라에몽  3.엘사  4.그 외" << endl;
	cin >> k;
	switch (k) {
	case 1:
		res = "image/img1.png";
		break;
	case 2:
		res = "image/img2.png";
		break;
	case 3:
		res = "image/img3.png";
		break;
	case 4:
		cout << "파일 경로를 입력해 주세요" << endl;
		cin >> res;
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