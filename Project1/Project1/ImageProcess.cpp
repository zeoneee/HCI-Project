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

// 흑백과 컬러 구분해서 하려햇으나 우리 눈엔 흑백인 이미지도 rgb채널을 가지고 있음
// 가지고 있는 이미지들은 전처리로 흑백으로 만들든 해야할 듯
Mat extract(Mat img) {
	cout << img.channels() << endl;

	Mat img_gray;
	cvtColor(img, img_gray, COLOR_BGR2GRAY);
	//imshow("img_gray", img_gray);

	// 컬러 도안
	if (img.channels() != 1) {
		Mat mask;	// 흑백 이미지를 임계값으로 이진화해 마스크 생성
		adaptiveThreshold(img_gray, mask, 255, THRESH_BINARY, ADAPTIVE_THRESH_GAUSSIAN_C, 5, 10);
		//imshow("mask", mask);

		// 해당 마스크의 역처리
		Mat mask_inv;
		bitwise_not(mask, mask_inv);

		// 워터마크 이미지에서 워터마크 영역만 추출(and 비트연산, 역 마스크 영역)
		Mat img_result;
		bitwise_and(img, img, img_result, mask = mask_inv);
		imshow("img_result", img_result);
		return img_result;

	}
	// 흑백 도안
	else {
		Mat img_binary;
		threshold(img_gray, img_binary, 0, 255, THRESH_BINARY | THRESH_OTSU);
		//imshow("img_binary", img_binary);

		CEdge ce;
		Mat img_edge = ce.GS_canny_edge_Canny(img_binary, 50, 100);
		imshow("img_edge", img_edge);
		return img_edge;
	}
}
