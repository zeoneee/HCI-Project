// Edge.h: interface for the CEdge class.
//
//////////////////////////////////////////////////////////////////////

#include<opencv2/opencv.hpp>

#if !defined(AFX_EDGE_H__F2A61D84_5347_4099_AEA6_2061F2B32778__INCLUDED_)
#define AFX_EDGE_H__F2A61D84_5347_4099_AEA6_2061F2B32778__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEdge
{
public:
	CEdge();
	virtual ~CEdge();

	// �Һ� ���� ���� : ����ũ(3x3)
	cv::Mat GS_sobel_edge(cv::Mat src_image, int method = 2);

	// �Һ� ���� ���� : cv::Sobel() �Լ� �̿�
	cv::Mat GS_sobel_edge_Sobel(cv::Mat src_image, int method = 2);

	// �Һ� ���� ���� : ����ũ(7x7)
	cv::Mat GS_sobel_edge_7x7(cv::Mat src_image, int method = 2);

	// �Һ� ���� ���� : �밢��
	cv::Mat GS_sobel_diagonal_edge(cv::Mat src_image, int method = 2);

	// ������ ���� ����
	cv::Mat GS_prewitt_edge(cv::Mat src_image, int method = 2);

	// ������þ ���� ���� 
	cv::Mat GS_freichen_edge(cv::Mat src_image, int method = 2);

	// ����ĳ��ƽ ���� ����
	cv::Mat GS_stochastic_Edge(cv::Mat src_image, int method = 2);

	// �ι��� ���� ����
	cv::Mat GS_roberts_edge(cv::Mat src_image, int method = 2);

	// ���ö�þ� ���� ���� : ����ũ
	cv::Mat GS_laplacian_edge(cv::Mat src_image, int method = 1);

	// ���ö�þ� ���� ���� : cv::Laplacian() �Լ� �̿�
	cv::Mat GS_laplacian_edge_Laplacian(cv::Mat src_image);

	// ���ö�þ� ������ 
	cv::Mat GS_laplacian_sharpening(cv::Mat src_image, int method = 1);

	double **GS_get_LoG_mask(double sigma, int *mask_height, int *mask_width);
	double GS_get_LoG_formular(double x, double sigma);
	double norm(double x, double y);
	double GS_get_distance(double a, double b, double c, double d);
	double gauss(double x, double sigma);

	// ĳ�� ���� ���� 
	cv::Mat GS_canny_edge(cv::Mat src_image, double sigma, int threshold, int method);

	// ĳ�� ���� ���� : cv::Canny() �Լ� �̿� 
	cv::Mat GS_canny_edge_Canny(cv::Mat src_image, double low_threshold = 50.0, double high_threshold = 100.0);

};
#endif // !defined(AFX_EDGE_H__F2A61D84_5347_4099_AEA6_2061F2B32778__INCLUDED_)
