#include <iostream>
#include "ImageProcess.h"
using namespace std;

string chooseImage() {
	int k;
	string res;

	cout << "� �̹����� �̿��Ͻðڽ��ϱ�?" << endl;
	cout << "1.¯��  2.���󿡸�  3.����  4.�� ��" << endl;
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
		cout << "���� ��θ� �Է��� �ּ���" << endl;
		cin >> res;
	default:
		cout << "�߸��� �Է��Դϴ�." << endl;
	}
	return res;
}

Mat extract(Mat img) {

	Mat img2gray; // �ռ��� �̹����� ������� ��ȯ 
	cvtColor(img, img2gray, CV_BGR2GRAY);

	Mat mask;	// ��� �̹����� �Ӱ谪���� ����ȭ�� ����ũ ����
	threshold(img2gray, mask, 250, 255, THRESH_BINARY);

	// �ش� ����ũ�� ��ó��
	Mat mask_inv;
	bitwise_not(mask, mask_inv);
	imshow("mask_inv", mask_inv);

	// ���͸�ũ �̹������� ���͸�ũ ������ ����(and ��Ʈ����, �� ����ũ ����)
	Mat img2_fg;
	bitwise_and(img, img, img2_fg, mask = mask_inv);
	imshow("img_small", img);
	
	return img2_fg;
}