// Filter.h: interface for the CFilter class.
//
//////////////////////////////////////////////////////////////////////

#include<opencv2/opencv.hpp>

#if !defined(AFX_FILTER_H__034717B5_0E25_4274_AE72_2FF2F9D09E29__INCLUDED_)
#define AFX_FILTER_H__034717B5_0E25_4274_AE72_2FF2F9D09E29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CFilter
{
public:
	CFilter();
	virtual ~CFilter();

	// ����þ� ������ ���͸�
	cv::Mat GS_gaussian_smoothing_filtering(cv::Mat src_image, double sigma);

//    // �ִ�/�ּҰ� ���͸�
//    IplImage *GS_maxmin_filtering(IplImage *src_image, int method, int mask_height = 3, int mask_width = 3);

//    // ��-trimmed ��հ� ���͸�
//    IplImage *GS_alpha_trim_mean_filtering(IplImage *src_image, double alpha, int mask_height = 3, int mask_width = 3);

	// ��հ� ���͸� 
	cv::Mat GS_mean_filtering(cv::Mat src_image, int mask_height = 3, int mask_width = 3);

//    // �̵�� ���͸�
//    IplImage *GS_median_filtering(IplImage *src_image, int mask_height = 3, int mask_width = 3, int type = 0);

	// ������ ����
	cv::Mat GS_soften_filter(cv::Mat src_image, int method);

	// ���ڽ� ��Ŀ��/������ ����
	cv::Mat GS_enhance_filter(cv::Mat src_image, int method);

	// �� ����Ʈ/���� ���� 
	cv::Mat GS_blue_lb_filter(cv::Mat src_image, int method);

	// ������
	cv::Mat GS_sharpening(cv::Mat src_image, int method);

	// ������ ���� ��� ���͸�
	cv::Mat GS_high_boost(cv::Mat src_image, float alpha, float bias = 0.0f);

	// �������
	cv::Mat GS_unsharpening(cv::Mat src_image, int method, float bias = 0.0f);

	// ����
	cv::Mat GS_blurring(cv::Mat src_image, int option, int method);

	// RGB �÷� ���������� �簢 ȿ��
	cv::Mat GS_rgb_embossing(cv::Mat src_image, int method);

	// HSV �÷� ���������� �簢 ȿ��
	cv::Mat GS_hsv_embossing(cv::Mat src_image, int method);

	// ��ϵ� ���󿡼��� �簢 ȿ��
	cv::Mat GS_embossing(cv::Mat src_image, int method);

	// ��ϵ� ���󿡼��� �簢 ȿ�� - ��ģ ȿ��
	cv::Mat GS_wild_embossing(cv::Mat src_image, int method);

	// ȸ�� ���� 
	cv::Mat GS_conv2(cv::Mat src_image, cv::Mat mat_kernel);

	// 1���� Ŀ���� ���� ���͸�.
	cv::Mat ApplyFilter2D(cv::Mat src_image,	/* ���� ���� */
					float *kernel,				/* 1���� Ŀ�� */
					int kHeight,				/* Ŀ���� ���� */
					int kWidth);				/* Ŀ���� �ʺ� */


	// 2���� Ŀ���� ���� ���͸�.
	cv::Mat ApplyFilter2D(cv::Mat src_image,	/* ���� ���� */
					  float **kernel,			/* 1���� Ŀ�� */
					  int kHeight,				/* Ŀ���� ���� */
					  int kWidth);				/* Ŀ���� �ʺ� */

	// CvMat�� kernel�� ���� ���͸�.
	cv::Mat ApplyFilter2D(cv::Mat src_image,		/* ���� ���� */
					  cv::Mat kernel);				/* Ŀ�� */

private:

	// ���� - cvFilter2D() �Լ� ���
	cv::Mat GS_blurring_cvFilter2D(cv::Mat src_image, int method);

	// ���� - cvSmooth() �Լ� ���
	cv::Mat GS_blurring_cvSmooth(cv::Mat src_image, int option, int method);
};

#endif // !defined(AFX_FILTER_H__034717B5_0E25_4274_AE72_2FF2F9D09E29__INCLUDED_)
