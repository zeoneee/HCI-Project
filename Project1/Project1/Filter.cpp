/*
* Filter.cpp : ���� ���� ���͸� ���� Ŭ����
* Modified by Lee, Moon-Ho (conv2@korea.com)
* Last Modified : 2006/11/15
*/

// Filter.cpp: implementation of the CFilter class.
//
//////////////////////////////////////////////////////////////////////

#include "Filter.h"
#include "Util.h"
#include "Color.h"
#include "Pixel.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFilter::CFilter()
{

}

CFilter::~CFilter()
{

}

// ����þ� ������ ���͸�
cv::Mat CFilter::GS_gaussian_smoothing_filtering(cv::Mat src_image, double sigma)
{
	int i, j, mask_height, mask_width;
	double var;
	CUtil cUtil;

	// �ʱ�ȭ 
	int height = src_image.rows;
	int width = src_image.cols;
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (src_image.channels() != 1) return dst_image;

	// sigma ������ �������� ���� ��
	if (sigma < 0.0)
	{
		//cUtil.GS_errMsg("sigma ���� 0.0 �̻��̾�� �մϴ�.");
		return dst_image;
	}

	// ����ũ�� ���Ѵ�.
	// Gaussian ����ũ�� ũ�⸦ �����Ѵ�.
	// length = 5 �̸� 2D�� ��� 5x5 ����ũ�̴�.
	int length = (int)(6 * sigma) + 1;
	int center = length / 2;

	mask_height = mask_width = length;
	float **mask = cUtil.GS_floatAlloc2D(mask_height, mask_width);
	
	for (i = 0; i<mask_height; i++)
	{
		for (j = 0; j<mask_width; j++)
		{
			var = (1.0 / (2.0*1*sigma*sigma)) // M_PI
				* exp(-((i - center)*(i - center) + (j - center)*(j - center))
					*(1.0 / (2.0*sigma*sigma)));

			mask[i][j] = (float)var;
		}
	}
	
	// 2���� ����ũ�� 1�������� ��ȯ
	float *single_mask = cUtil.GS_toSingleArray(mask, mask_height, mask_width);

	// CvMat���� ��ȯ
	cv::Mat mat_kernel = cv::Mat(mask_height, mask_width, CV_32FC1, single_mask);

	// ȸ�� �� dst_image�� ����
	cv::Mat tmp_image = GS_conv2(src_image, mat_kernel);
	tmp_image.copyTo(dst_image);

	free(single_mask);
	tmp_image.release();
	cUtil.GS_free2D(mask, mask_height);

	return dst_image;
}

// �ִ�/�ּҰ� ���͸�

// ��-trimmed ��հ� ���͸�
cv::Mat CFilter::GS_mean_filtering(cv::Mat src_image, int mask_height, int mask_width)
{
	int i, j, m, n;
	CUtil cUtil;
	double sum, var;

	// �ʱ�ȭ 
	int height = src_image.rows;
	int width = src_image.cols;
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels() != 1) return dst_image;

	for (i = 0; i<height - mask_height + ((mask_height - 1) / 2); i++)
	{
		for (j = 0; j<width - mask_width + ((mask_width - 1) / 2); j++)
		{
			sum = 0.0;

			if ((i + mask_height > height) || (j + mask_width > width))
				continue;

			// mask_height x mask_width�� ����� ��� mask�� �Ҵ�.
			for (m = 0; m<mask_height; m++)
			{
				for (n = 0; n<mask_width; n++)
				{
					sum += src_image.at<uchar>(m + i, n + j);
				}
			}

			// ��հ��� ���Ѵ�.
			var = sum / (mask_height*mask_width);
			dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1)) = var;
			
		}
	}

	return dst_image;
}

// �̵�� ���͸�



// �� ����Ʈ/���� ���� 
cv::Mat CFilter::GS_blue_lb_filter(cv::Mat src_image, int method)
{
	CUtil cUtil;

	int mask_height = 3;
	int mask_width = 3;

	float **mask = cUtil.GS_floatAlloc2D(mask_height, mask_width);

	// �� ����Ʈ ����
	if (method == 0)
	{
		mask[0][0] = 1.0f; mask[0][1] = 2.0f; mask[0][2] = 1.0f;
		mask[1][0] = 2.0f; mask[1][1] = 2.0f; mask[1][2] = 2.0f;
		mask[2][0] = 1.0f; mask[2][1] = 2.0f; mask[2][2] = 1.0f;
	}
	// �� ���� ���� 
	else if (method == 1)
	{
		mask[0][0] = 1.0f; mask[0][1] = 2.0f; mask[0][2] = 1.0f;
		mask[1][0] = 2.0f; mask[1][1] = 4.0f; mask[1][2] = 2.0f;
		mask[2][0] = 1.0f; mask[2][1] = 2.0f; mask[2][2] = 1.0f;
	}

	// 2���� ����ũ�� 1�������� ��ȯ
	float *single_mask = cUtil.GS_toSingleArray(mask, mask_height, mask_width);

	// cv::Mat���� ��ȯ
	cv::Mat mat_kernel = cv::Mat(mask_height, mask_width, CV_32FC1, single_mask);

	float tmp;
	for (int i = 0; i<mask_height; i++)
	{
		for (int j = 0; j<mask_width; j++)
		{
			tmp = (float)mat_kernel.at<float>(i, j);

			if (method == 0) //cvmSet(&mat_kernel, i, j, tmp / 14.0f);
				mat_kernel.at<float>(i, j) = tmp / 14.0f;
			else if (method == 1) //cvmSet(&mat_kernel, i, j, tmp / 16.0f);
				mat_kernel.at<float>(i, j) = tmp / 16.0f;
		}
	}

	// ȸ�� 
	cv::Mat dst_image = GS_conv2(src_image, mat_kernel);

	// �Ҵ��� �޸� ����
	cUtil.GS_free1D(single_mask);
	cUtil.GS_free2D(mask, mask_height);

	return dst_image;
}

// ���ڽ� ��Ŀ��/������ ����
cv::Mat CFilter::GS_enhance_filter(cv::Mat src_image, int method)
{
	CUtil cUtil;

	int mask_height = 3;
	int mask_width = 3;

	float **mask = cUtil.GS_floatAlloc2D(mask_height, mask_width);

	// ���ڽ� ��Ŀ�� ����
	if (method == 0)
	{
		mask[0][0] = -1.0f; mask[0][1] = 0.0f; mask[0][2] = -1.0f;
		mask[1][0] = 0.0f; mask[1][1] = 7.0f; mask[1][2] = 0.0f;
		mask[2][0] = -1.0f; mask[2][1] = 0.0f; mask[2][2] = -1.0f;
	}
	// ���ڽ� ������ ���� 
	else if (method == 1)
	{
		mask[0][0] = 0.0f; mask[0][1] = -1.0f; mask[0][2] = 0.0f;
		mask[1][0] = -1.0f; mask[1][1] = 9.0f; mask[1][2] = -1.0f;
		mask[2][0] = 0.0f; mask[2][1] = -1.0f; mask[2][2] = 0.0f;
	}

	// 2���� ����ũ�� 1�������� ��ȯ
	float *single_mask = cUtil.GS_toSingleArray(mask, mask_height, mask_width);

	// CvMat���� ��ȯ
	cv::Mat mat_kernel = cv::Mat(mask_height, mask_width, CV_32FC1, single_mask);
	

	float tmp;
	for (int i = 0; i<mask_height; i++)
	{
		for (int j = 0; j<mask_width; j++)
		{
			tmp = (float)mat_kernel.at<float>(i, j);

			if (method == 0) //cvmSet(&mat_kernel, i, j, tmp / 3.0f);
				mat_kernel.at<float>(i, j) = tmp / 3.0f;
			else if (method == 1) //cvmSet(&mat_kernel, i, j, tmp / 5.0f);
				mat_kernel.at<float>(i, j) = tmp / 5.0f;
		}
	}

	// ȸ�� 
	cv::Mat dst_image = GS_conv2(src_image, mat_kernel);

	// �Ҵ��� �޸� ����
	cUtil.GS_free1D(single_mask);
	cUtil.GS_free2D(mask, mask_height);

	return dst_image;
}

// ������ ����
cv::Mat CFilter::GS_soften_filter(cv::Mat src_image, int method)
{
	CUtil cUtil;

	int mask_height = 3;
	int mask_width = 3;

	float **mask = cUtil.GS_floatAlloc2D(mask_height, mask_width);

	if (method == 0)
	{
		mask[0][0] = 11.0f; mask[0][1] = 11.0f; mask[0][2] = 11.0f;
		mask[1][0] = 11.0f; mask[1][1] = 11.0f; mask[1][2] = 11.0f;
		mask[2][0] = 11.0f; mask[2][1] = 11.0f; mask[2][2] = 11.0f;
	}
	else if (method == 1)
	{
		mask[0][0] = 1.0f; mask[0][1] = 1.0f; mask[0][2] = 1.0f;
		mask[1][0] = 1.0f; mask[1][1] = 10.0f; mask[1][2] = 1.0f;
		mask[2][0] = 1.0f; mask[2][1] = 1.0f; mask[2][2] = 1.0f;
	}
	else if (method == 2)
	{
		mask[0][0] = 1.0f; mask[0][1] = 1.0f; mask[0][2] = 1.0f;
		mask[1][0] = 1.0f; mask[1][1] = 4.0f; mask[1][2] = 1.0f;
		mask[2][0] = 1.0f; mask[2][1] = 1.0f; mask[2][2] = 1.0f;
	}

	// 2���� ����ũ�� 1�������� ��ȯ
	float *single_mask = cUtil.GS_toSingleArray(mask, mask_height, mask_width);

	// CvMat���� ��ȯ
	cv::Mat mat_kernel = cv::Mat(mask_height, mask_width, CV_32FC1, single_mask);

	float tmp;
	for (int i = 0; i<mask_height; i++)
	{
		for (int j = 0; j<mask_width; j++)
		{
			tmp = mat_kernel.at<float>(i, j);

			if (method == 0) // cvmSet(&mat_kernel, i, j, tmp / 99.0f);
				mat_kernel.at<float>(i, j) = tmp / 99.0f;
			else if (method == 1) // cvmSet(&mat_kernel, i, j, tmp / 18.0f);
				mat_kernel.at<float>(i, j) = tmp / 18.0f;
			else if (method == 2) // cvmSet(&mat_kernel, i, j, tmp / 12.0f);
				mat_kernel.at<float>(i, j) = tmp / 12.0f;
		}
	}

	// ȸ�� 
	cv::Mat dst_image = GS_conv2(src_image, mat_kernel);

	// �Ҵ��� �޸� ����
	cUtil.GS_free1D(single_mask);
	cUtil.GS_free2D(mask, mask_height);

	return dst_image;
}

// ������
cv::Mat CFilter::GS_sharpening(cv::Mat src_image, int method)
{
	CUtil cUtil;

	int mask_height = 3;
	int mask_width = 3;

	float **mask = cUtil.GS_floatAlloc2D(mask_height, mask_width);

	if (method == 0)
	{
		mask[0][0] = 1.0f; mask[0][1] = -2.0f; mask[0][2] = 1.0f;
		mask[1][0] = -2.0f; mask[1][1] = 5.0f; mask[1][2] = -2.0f;
		mask[2][0] = 1.0f; mask[2][1] = -2.0f; mask[2][2] = 1.0f;
	}
	else if (method == 1)
	{
		mask[0][0] = 0.0f; mask[0][1] = -1.0f; mask[0][2] = 0.0f;
		mask[1][0] = -1.0f; mask[1][1] = 5.0f; mask[1][2] = -1.0f;
		mask[2][0] = 0.0f; mask[2][1] = -1.0f; mask[2][2] = 0.0f;
	}
	else if (method == 2)
	{
		mask[0][0] = -1.0f; mask[0][1] = -1.0f; mask[0][2] = -1.0f;
		mask[1][0] = -1.0f; mask[1][1] = 9.0f; mask[1][2] = -1.0f;
		mask[2][0] = -1.0f; mask[2][1] = -1.0f; mask[2][2] = -1.0f;
	}
	// ��ϴ�� ���� ����ũ�� �˷��� �ִ�.
	else if (method == 3)
	{
		mask[0][0] = 1.0f; mask[0][1] = 1.0f; mask[0][2] = 1.0f;
		mask[1][0] = 1.0f; mask[1][1] = -7.0f; mask[1][2] = 1.0f;
		mask[2][0] = 1.0f; mask[2][1] = 1.0f; mask[2][2] = 1.0f;
	}
	else if (method == 4)
	{
		mask[0][0] = 0.0f;      mask[0][1] = -1.0f / 6.0f;  mask[0][2] = 0.0f;
		mask[1][0] = -1.0f / 6.0f; mask[1][1] = 10.0f / 6.0f;  mask[1][2] = -1.0f / 6.0f;
		mask[2][0] = 0.0f;      mask[2][1] = -1.0f / 6.0f;   mask[2][2] = 0.0f;
	}
	else if (method == 5)
	{
		mask[0][0] = 0.0f;      mask[0][1] = -1.0 / 4.0f;  mask[0][2] = 0.0f;
		mask[1][0] = -1.0f / 4.0f; mask[1][1] = 8.0f / 4.0f; mask[1][2] = -1.0f / 4.0f;
		mask[2][0] = 0.0f;      mask[2][1] = -1.0 / 4.0f;  mask[2][2] = 0.0f;
	}

	// 2���� ����ũ�� 1�������� ��ȯ
	float *single_mask = cUtil.GS_toSingleArray(mask, mask_height, mask_width);

	// CvMat���� ��ȯ
	cv::Mat mat_kernel = cv::Mat(mask_height, mask_width, CV_32FC1, single_mask);

	// ȸ�� 
	cv::Mat dst_image = GS_conv2(src_image, mat_kernel);

	// �Ҵ��� �޸� ����
	cUtil.GS_free1D(single_mask);
	cUtil.GS_free2D(mask, mask_height);

	return dst_image;
}

// ������ ���� ��� ���͸�
cv::Mat CFilter::GS_high_boost(cv::Mat src_image, float alpha, float bias)
{
	CUtil cUtil;

	int mask_height = 3;
	int mask_width = 3;
	int mask_mid_height = (mask_height + 1) / 2;
	int mask_mid_width = (mask_width + 1) / 2;

	// ����ũ ��� ����
	float mask_weights = (float)(mask_height*mask_width);

	// ����ũ �ʱ�ȭ
	cv::Mat mat_kernel = cv::Mat(cv::Size(mask_width, mask_height),
						CV_32FC1,
						cv::Scalar(-alpha/mask_weights));

	// �߽� ��� ����
	float mask_mid_var = (float)(mask_weights * alpha - 1.0) / mask_weights;
	mat_kernel.at<float>(mask_mid_height, mask_mid_width) = mask_mid_var;
	// ȸ�� 
	cv::Mat tmp_image = GS_conv2(src_image, mat_kernel);

	// bias��ŭ �����ش�.
	cv::Mat dst_image = tmp_image.clone();
	cv::add(tmp_image, cv::Scalar(bias), dst_image);

	// �Ҵ��� �޸� ���� 
	mat_kernel.release();
	tmp_image.release();

	return dst_image;
}

// �������
cv::Mat CFilter::GS_unsharpening(cv::Mat src_image, int method, float bias)
{
	CPixel cPixel;

	// ������ ��� ���͸� : ����
	cv::Mat blurring_image = GS_blurring(src_image, 0, method);

	// �� ���� - ������ ��� ���͸� ����
	cv::Mat tmp_image = cPixel.GS_subtract_image(src_image, blurring_image);

	// bias ��ŭ �����ش�.
	cv::Mat dst_image = tmp_image.clone();
	cv::add(tmp_image, cv::Scalar(bias), dst_image);

	// �Ҵ��� �޸� ����
	tmp_image.release();

	return dst_image;
}

// ����
cv::Mat CFilter::GS_blurring(cv::Mat src_image, int option, int method)
{
	cv::Mat dst_image;

	// ���� ������ �Լ� ���
	if (option == 0)
	{
		dst_image = GS_blurring_cvFilter2D(src_image, method);
	}
	// OpenCV �⺻ ���� �Լ� ���
	else
	{
		dst_image = GS_blurring_cvSmooth(src_image, option, method);
	}

	return dst_image;
}

// ���� - ���� ����
cv::Mat CFilter::GS_blurring_cvFilter2D(cv::Mat src_image, int method)
{
	CUtil cUtil;
	int mask_height, mask_width;

	// method : 0�̸� 3x3 ����ũ, 1�̸� 5x5 ����ũ, 2�̸� 7x7 ����ũ
	if (method == 0) mask_height = mask_width = 3;
	else if (method == 1) mask_height = mask_width = 5;
	else if (method == 2) mask_height = mask_width = 7;

	// ����ũ �ʱ�ȭ
	cv::Mat mat_kernel = cv::Mat(mask_height, mask_width, CV_32FC1, cv::Scalar(0));

	// ����ũ�� ��� ����
	int mask_length = mask_height * mask_width;
	for (int i = 0; i<mask_height; i++)
		for (int j = 0; j<mask_width; j++)
			mat_kernel.at<float>(i, j) = 1.0f / (float)mask_length;

	// ȸ�� 
	cv::Mat dst_image = GS_conv2(src_image, mat_kernel);

	// �Ҵ��� �޸� ���� 
	mat_kernel.release();

	return dst_image;
}

// ���� - OpenCV �⺻ ���� �Լ� ���
cv::Mat CFilter::GS_blurring_cvSmooth(cv::Mat src_image, int option, int method)
{
	int mask_height, mask_width;

	// �ʱ�ȭ
	cv::Mat dst_image = cv::Mat(src_image.size(), src_image.type());

	// method : 0�̸� 3x3 ����ũ, 1�̸� 5x5 ����ũ, 2�̸� 7x7 ����ũ
	if (method == 0) mask_height = mask_width = 3;
	else if (method == 1) mask_height = mask_width = 5;
	else if (method == 2) mask_height = mask_width = 7;

	// option �� üũ
	// option : 1 - CV_BLUR,   
	// option : 2 - CV_GAUSSIAN
	// option : 3 - CV_MEDIAN
	switch (option) {
	case 1: // CV_BLUR
		cv::blur(src_image, dst_image, cv::Size(mask_width, mask_height));
		break;
	case 2: // CV_GAUSSIAN
		cv::GaussianBlur(src_image, dst_image, cv::Size(mask_width, mask_height), 0, 0);
		break;
	case 3: // CV_MEDIAN
		cv::medianBlur(src_image, dst_image, 3); // ksize�� �ٽ� �����ؾ���.
		break;
	case 4: // CV_BILATERAL
		cv::bilateralFilter(src_image, dst_image, -1 , 100, 100); // d<0 �̸� d ���� Sigmaspace�κ��� ����.
	}

	return dst_image;
}

// ��ϵ� ���󿡼��� �簢 ȿ�� - ��ģ ȿ��
cv::Mat CFilter::GS_wild_embossing(cv::Mat src_image, int method)
{
	CUtil cUtil;
	int mask_length = 9;

	// North
	float mask_north[] = { -1.0f, -2.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 1.0f };
	float mask_west[] = { -1.0f, 0.0f, 1.0f, -2.0f, 0.0f, 2.0f, -1.0f, 0.0f, 1.0f };
	float mask_south[] = { 1.0, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, -1.0f, -2.0f, -1.0f };
	float mask_east[] = { 1.0f, 0.0f, -1.0f, 2.0f, 0.0f, -2.0f, 1.0f, 0.0f, -1.0f };
	float mask_northeast[] = { 0.0f, -1.0f, -2.0f, 1.0f, 0.0f, -1.0f, 2.0f, 1.0f, 0.0f };


	// 1���� �޸� �Ҵ�
	float *mask = cUtil.GS_floatAlloc1D(mask_length);

	// ����ũ ����
	if (method == 0)
	{
		memcpy(mask, mask_north, sizeof(float)*mask_length);
	}
	else if (method == 1)
	{
		memcpy(mask, mask_west, sizeof(float)*mask_length);
	}
	else if (method == 2)
	{
		memcpy(mask, mask_south, sizeof(float)*mask_length);
	}
	else if (method == 3)
	{
		memcpy(mask, mask_east, sizeof(float)*mask_length);
	}
	else if (method == 4)
	{
		memcpy(mask, mask_northeast, sizeof(float)*mask_length);
	}

	// CvMat���� ��ȯ
	cv::Mat mat_kernel = cv::Mat(3, 3, CV_32FC1, mask);

	// ȸ�� 
	cv::Mat tmp_image = GS_conv2(src_image, mat_kernel);

	// �� ������ ��հ��� ���Ѵ�.
	cv::Scalar avg = cv::mean(src_image);

	// ��հ��� ���Ѵ�.
	cv::Mat dst_image = tmp_image.clone();
	cv::add(tmp_image, avg, dst_image);

	// �Ҵ��� �޸� ����
	tmp_image.release();
	cUtil.GS_free1D(mask);

	return dst_image;
}

// HSV �÷� ���������� �簢 ȿ��
cv::Mat CFilter::GS_hsv_embossing(cv::Mat src_image, int method)
{
	// ä�� �и�
	cv::Mat rgb[3]; // ���� ! 0 - blue 1 - green 2 - red
	cv::Mat hsv[3]; //0 - hue  1 - saturation 2 - value

	CColor cColor;
	cColor.GS_splitRGB(src_image, rgb);
	cColor.GS_rgb2hsv(rgb, hsv);

	// �簢 ȿ�� ����
	cv::Mat value_embossing = GS_embossing(hsv[3], method);

	cv::Mat new_rgb[3];
	hsv[2] = value_embossing.clone();
	cColor.GS_hsv2rgb(hsv, new_rgb);

	cv::Mat dst_image = cColor.GS_compositeRGB(new_rgb);

	// �Ҵ��� �޸� ����
	rgb->release(); new_rgb->release();

	hsv->release();
	value_embossing.release();

	return dst_image;
}

// RGB �÷� ���������� �簢 ȿ��
cv::Mat CFilter::GS_rgb_embossing(cv::Mat src_image, int method)
{
	/** ���� : cvFilter2D() �Լ��� 3�������� ó���� �� �����Ƿ�,
	�Ʒ� �ڵ� ��� �ٲ㵵 �Ȱ���.
	return GS_embossing( src_image, method );
	*/

	CColor cColor;

	// ä�� �и�
	cv::Mat rgb[3];
	cColor.GS_splitRGB(src_image, rgb);

	// �簢 ȿ�� ����
	// ä�� �и��� rgb ���� �ƴ� bgr ������ �ȴٴ� ���� ����
	cv::Mat blue_embossing = GS_embossing(rgb[0], method);
	cv::Mat green_embossing = GS_embossing(rgb[1], method);
	cv::Mat red_embossing = GS_embossing(rgb[2], method);
	

	cv::Mat dst_image = cColor.GS_compositeRGB(rgb);

	// �Ҵ��� �޸� ����
	rgb->release();
	red_embossing.release();
	green_embossing.release();
	blue_embossing.release();

	return dst_image;
}

// ��ϵ� ���󿡼��� �簢 ȿ��
cv::Mat CFilter::GS_embossing(cv::Mat src_image, int method)
{
	CUtil cUtil;
	int mask_height = 3;
	int mask_width = 3;
	int mask_length = mask_height * mask_width;

	// 135��
	float mask_135[] = { -1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f };
	// 90��
	float mask_90[] = { 0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f };

	// 45�� 
	float mask_45[] = { 0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f };

	// 1���� �޸� �Ҵ�
	float *mask = cUtil.GS_floatAlloc1D(mask_length);

	// ����ũ ����
	if (method == 0)
	{
		memcpy(mask, mask_135, sizeof(float)*mask_length);
	}
	else if (method == 1)
	{
		memcpy(mask, mask_90, sizeof(float)*mask_length);
	}
	else if (method == 2)
	{
		memcpy(mask, mask_45, sizeof(float)*mask_length);
	}

	cv::Mat mat_kernel = cv::Mat(cv::Size(mask_width, mask_height), CV_32FC1, mask);

	// ȸ�� 
	cv::Mat tmp_image = GS_conv2(src_image, mat_kernel);

	// �� ������ ��հ��� ���Ѵ�.
	cv::Scalar avg = mean(src_image, NULL);

	// ��հ��� ���Ѵ�.
	cv::Mat dst_image = tmp_image.clone();
	cv::add(tmp_image, avg, dst_image);

	// �Ҵ��� �޸� ����
	tmp_image.release();
	cUtil.GS_free1D(mask);

	return dst_image;
}

// ȸ�� ����
cv::Mat CFilter::GS_conv2(cv::Mat src_image, cv::Mat mat_kernel)
{
	// ȸ�� ����(=���͸�) ����� ���� ���� ���� �ʱ�ȭ
	cv::Mat dst_image = cv::Mat(src_image.size(), src_image.type());

	// ���͸� ���� : ����ũ�� ���� ȸ��(convolution)
	cv::filter2D(src_image, dst_image, dst_image.depth(), mat_kernel);

	return dst_image;
}

// 1���� Ŀ���� ���� ���͸�. - ��
cv::Mat CFilter::ApplyFilter2D(cv::Mat src_image, float * kernel, int kHeight, int kWidth)
{
	// cv::Mat���� ��ȯ.
	cv::Mat mat_kernel = cv::Mat(cv::Size(kWidth, kHeight), CV_32FC1, kernel);

	// �����ε��� ��� �Լ� �� ȣ��
	cv::Mat dst_image = ApplyFilter2D(src_image, mat_kernel);

	// �Ҵ��� �޸� ���� 
	mat_kernel.release();

	return dst_image;
}

// 2���� Ŀ���� ���� ���͸�. - ��
cv::Mat CFilter::ApplyFilter2D(cv::Mat src_image, float ** kernel, int kHeight, int kWidth)
{
	int cnt = -1;

	// 2������ 1�������� ��ȯ.
	float *single_kernel = (float *)calloc(kHeight*kWidth, sizeof(float));

	for (int i = 0; i<kHeight; i++)
	{
		for (int j = 0; j<kWidth; j++)
		{
			single_kernel[++cnt] = kernel[i][j];
		}
	}

	// �����ε��� ��� �Լ� �� ȣ��.
	cv::Mat dst_image = ApplyFilter2D(src_image, single_kernel, kHeight, kWidth);

	// �Ҵ��� �޸� ����
	free(single_kernel);

	return dst_image;
}

// CvMat�� kernel�� ���� ���͸�. - ��
cv::Mat CFilter::ApplyFilter2D(cv::Mat src_image, cv::Mat kernel)
{
	// ���͸� ����� ���� ���� ���� �ʱ�ȭ
	cv::Mat dst_image = cv::Mat(src_image.size(), src_image.type());

	// ���͸� ���� : kernel�� ���� ȸ��(convolution)
	cv::filter2D(src_image, dst_image, -1 , kernel);

	return dst_image;
}


