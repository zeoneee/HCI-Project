// Color.h: interface for the CColor class.
//
//////////////////////////////////////////////////////////////////////

#include<opencv2/opencv.hpp>

#if !defined(AFX_COLOR_H__51E6F4C5_45B6_440E_8240_FCFF2CFD7CAC__INCLUDED_)
#define AFX_COLOR_H__51E6F4C5_45B6_440E_8240_FCFF2CFD7CAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CColor
{
public:
	CColor();
	virtual ~CColor();

	// RGB �÷� ���� �и�, ���࿡ ��ϵ� �����̸� �� ä�ο� �Ҵ���.
	void GS_splitRGB(cv::Mat rgb_image, cv::Mat* rgb);

	// RGB ä���� �����Ͽ� �ϳ��� RGB �÷� �������� ��ȯ�Ѵ�.
	cv::Mat GS_compositeRGB(cv::Mat* rgb);

	// RGB �÷� ������ HSV �÷� �������� ��ȯ�Ѵ�.
	void GS_rgb2hsv(cv::Mat* rgb, cv::Mat* hsv);

	// HSV �÷� ������ RGB �÷� �������� ��ȯ�Ѵ�.
	void GS_hsv2rgb(cv::Mat* hsv, cv::Mat* rgb);

	// RGB �÷� ������ YCbCr �÷� �������� ��ȯ�Ѵ�.
	void GS_rgb2ycbcr(cv::Mat* rgb, cv::Mat* yCbCr);

	// YCbCr �÷� ������ RGB �÷� �������� ��ȯ�Ѵ�.
	void GS_ycbcr2rgb(cv::Mat* YCbCr, cv::Mat* rgb);

	// �־��� code�� ���� �÷� ���� ��ȯ�� ��, �� ä���� �и��Ͽ� ��ȯ�Ѵ�.
	void GS_cvtColor(cv::Mat* src, cv::Mat* dst, int codes);

	// �÷� ������ ��ϵ� �������� ��ȯ 
	cv::Mat GS_rgb2gray(cv::Mat rgb_image);

	// �÷� ���� -> ä�� �и�
	void SplitRGB(cv::Mat color_image,	/* ���� RGB ���� */
		cv::Mat* rgb);		/* bgr ä�� �迭 */
};

#endif // !defined(AFX_COLOR_H__51E6F4C5_45B6_440E_8240_FCFF2CFD7CAC__INCLUDED_)
