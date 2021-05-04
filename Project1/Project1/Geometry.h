// Geometry.h: interface for the CGeometry class.
//
//////////////////////////////////////////////////////////////////////

//#include<opencv\cv.h>
//#include<opencv\cxcore.h>
//#include<opencv\cvaux.h>
//#include<opencv\highgui.h>
#include <opencv2/opencv.hpp>


#if !defined(AFX_GEOMETRY_H__1E8A5F13_A043_489A_A703_835E78F64E03__INCLUDED_)
#define AFX_GEOMETRY_H__1E8A5F13_A043_489A_A703_835E78F64E03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGeometry
{
public:
	CGeometry();
	virtual ~CGeometry();

/*
	// �ܼ� Ȯ��/���
	IplImage *GS_enlarge(IplImage *src_image, int resize_height, int resize_width);

	// �ֱ��� �̿� ������
	IplImage *GS_neighbor_interp(IplImage *src_image, int resize_height, int reisize_width);

	// �缱�� ������
	IplImage *GS_bilinear_interp(IplImage *src_image, int resize_height, int reisize_width);

	// ���� : �߰���/��հ�
	IplImage *GS_reduction(IplImage *src_image, double h_scale, double w_scale, int method);

	// ȸ��
	IplImage *GS_rotate(IplImage *src_image, int angle);
*/

	// ������ ��ȯ
	cv::Mat GS_affine(cv::Mat src_image, cv::Mat map_matrix);

	// ������ ��ȯ - ȸ��
	cv::Mat GS_affineRotate(cv::Mat src_image, double angle, double scale);

	// ���� ��ȯ
	cv::Mat GS_perspective(cv::Mat src_image, cv::Point2f src_point, cv::Point2f dst_point);

	// �ݻ� �Ǵ� ��Ī
	cv::Mat GS_reflect(cv::Mat src_image, int flip_mode);

	// ���� ���� ����
	cv::Mat GS_getROI(cv::Mat src_image, int startH, int startW, int height, int width);

	// ���� ���� ����� ���� ������� ���� 
	// cv::Mat �ڷ����� �ش� ��� ����.
//    void GS_resetROI(IplImage *roi_image);
};

#endif // !defined(AFX_GEOMETRY_H__1E8A5F13_A043_489A_A703_835E78F64E03__INCLUDED_)
