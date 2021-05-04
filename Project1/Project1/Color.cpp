// Color.h: interface for the CColor class.
//
//////////////////////////////////////////////////////////////////////


#include "Color.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CColor::CColor()
{

}

CColor::~CColor()
{

}

// �÷� ������ ��ϵ� �������� ��ȯ  (conversion color space to gray scale image)
cv::Mat CColor::GS_rgb2gray(cv::Mat rgb_image)
{
	cv::Mat gray_image = cv::Mat(rgb_image.size(), CV_8UC1);

	cv::cvtColor(rgb_image, gray_image, cv::COLOR_BGR2GRAY);

	return gray_image;
}



// RGB �÷� ������ HSV �÷� �������� ��ȯ�Ѵ�. (convert RGB color space to HSV color space)
void CColor::GS_rgb2hsv(cv::Mat* rgb, cv::Mat* hsv)
{
	GS_cvtColor(rgb, hsv, cv::COLOR_BGR2HSV);

	return;
}

// �־��� code�� ���� �÷� ���� ��ȯ�� ��, �� ä���� �и��Ͽ� ��ȯ�Ѵ�. 
//(split each channel and return after converting color space according to the given code) 
void CColor::GS_cvtColor(cv::Mat* src, cv::Mat* dst, int code)
{
	cv::Mat src_image = cv::Mat(src[0].size(), src[0].type());
	cv::Mat dst_image = cv::Mat(src[0].size(), src[0].type());

	for (int i = 0; i < 3; i++)
	{
		dst[i] = cv::Mat(src[0].size(), src[0].type());
	}

	// STEP 2 : �� ä���� src_image�� �����Ѵ�.  (composite each channel to src_image)
	cv::merge(src, 3, src_image);

	// STEP 3 : �ش� �÷� ������ �־��� ���ڿ� �´� �÷� �������� ��ȯ. (convert color space to the new one which meets given argument)
	cv::cvtColor(src_image, dst_image, code);

	// STEP 4 : ��ȯ�� �÷� ������ �и�(split the converted color space)
	cv::split(dst_image, dst);

	// STEP 5 : �޸� ���� (memory deallocation)
	src_image.release();
	dst_image.release();

	return;

}

// HSV �÷� ������ RGB �÷� �������� ��ȯ�Ѵ�.  (convert RGB color space to HSV color space)
void CColor::GS_hsv2rgb(cv::Mat* hsv, cv::Mat* rgb)
{
	GS_cvtColor(hsv, rgb, cv::COLOR_HSV2BGR);
}

// RGB �÷� ������ YCbCr �÷� �������� ��ȯ�Ѵ�.  (convert RGB color space to YCbCr color space)
void CColor::GS_rgb2ycbcr(cv::Mat* rgb, cv::Mat* yCbCr)
{
	GS_cvtColor(rgb, yCbCr, cv::COLOR_BGR2YCrCb);
}

// YCbCr �÷� ������ RGB �÷� �������� ��ȯ�Ѵ�.  (convert RGB color space to YCbCr color space)
void CColor::GS_ycbcr2rgb(cv::Mat* YCbCr, cv::Mat* rgb)
{
	GS_cvtColor(YCbCr, rgb, cv::COLOR_YCrCb2BGR);
}

// RGB �÷� ���� �и�, ���࿡ ��ϵ� �����̸� �� ä�ο� �Ҵ��Ѵ�. (split RGB color space/ if the image is gray scale, assign it to each channel) 
void CColor::GS_splitRGB(cv::Mat rgb_image, cv::Mat* rgb)
{
	// STEP 1 : �޸� �Ҵ� (memory allocation)
	for (int i = 0; i < 3; i++) {
		rgb[i] = cv::Mat(rgb_image.size(), rgb_image.type());
	}

	// STEP 2 : cvCvtPixToPlane() �Լ� ȣ�� (call cvCvtPixToPlne() mothod)
	// �÷� ���� (color image)
	if (rgb_image.channels() == 3)
	{
		// ���� : BGR ä�� ������ ������. (caution: B->G->R channel order)
		cv::split(rgb_image, rgb);
	}
	// ��ϵ� ���� (gray scale image)
	else if (rgb_image.channels() == 1)
	{
		cv::Mat tmp_image = cv::Mat(rgb_image.size(), CV_8UC3);

		cv::cvtColor(rgb_image, tmp_image, cv::COLOR_GRAY2BGR);

		cv::split(tmp_image, rgb);

		tmp_image.release();
	}

	return;
}


// RGB ä���� �����Ͽ� RGB �÷� �������� ��ȯ�Ѵ�. (composite RGB channels and convert RGB color space)
cv::Mat CColor::GS_compositeRGB(cv::Mat* rgb)
{
	cv::Mat rgb_image = cv::Mat(rgb[0].size(), CV_8UC3);

	cv::merge(rgb, 3, rgb_image);

	return rgb_image;
}

// RGB �÷� ���� �и� - 5�� ���� (Split RGB color space)
void CColor::SplitRGB(cv::Mat color_image, cv::Mat* rgb)
{
	rgb[0] = cv::Mat(color_image.size(), CV_8UC1);
	rgb[1] = cv::Mat(color_image.size(), CV_8UC1);
	rgb[2] = cv::Mat(color_image.size(), CV_8UC1);

	cv::split(color_image, rgb);
}

