/*
* Util.cpp : ��ƿ��Ƽ ���� Ŭ����
* Modified by Lee, Moon-Ho (conv2@korea.com)
* Last Modified : 2006/11/16
*/

#include "Util.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUtil::CUtil()
{

}

CUtil::~CUtil()
{

}





// 2������ 1�������� ��ȯ
float *CUtil::GS_toSingleArray(float **data, int height, int width)
{
	int cnt = -1;
	float *single_data = GS_floatAlloc1D(height*width);

	for (int i = 0; i<height; i++)
	{
		for (int j = 0; j<width; j++)
		{
			single_data[++cnt] = data[i][j];
		}
	}

	return single_data;
}

// 2���� �޸� �Ҵ�
float **CUtil::GS_floatAlloc2D(int height, int width)
{
	float **data;

	data = (float **)calloc(height, sizeof(float *));
	for (int i = 0; i<height; i++)
	{
		data[i] = (float *)calloc(width, sizeof(float));
	}

	return data;
}

// 2���� �޸� �Ҵ�
double **CUtil::GS_doubleAlloc2D(int height, int width)
{
	double **data;

	data = (double **)calloc(height, sizeof(double *));
	for (int i = 0; i<height; i++)
	{
		data[i] = (double *)calloc(width, sizeof(double));
	}

	return data;
}

// �Ҵ��� 2���� �޸� ����
void CUtil::GS_free2D(double **data, int length)
{
	for (int i = 0; i<length; i++)
		free(data[i]);

	free(data);
}

// �Ҵ��� 2���� �޸� ����
void CUtil::GS_free2D(float **data, int length)
{
	for (int i = 0; i<length; i++)
		free(data[i]);

	free(data);
}

// 1���� �޸� �Ҵ�
float *CUtil::GS_floatAlloc1D(int length)
{
	return (float *)calloc(length, sizeof(float));
}

// �Ҵ��� 1���� �޸� ����
void CUtil::GS_free1D(float *data)
{
	if (!data) free(data);
}

cv::Mat CUtil::GS_createImage(cv::Size size, int nChannels)
{
	uchar type = ((nChannels - 1) << 3) + 0;
	cv::Mat dst_image = cv::Mat(size, type);
	return dst_image;
}

// ������ ũ�� ��������, ������ ���̰� 4�� ����� �°� ���/Ȯ��
cv::Mat CUtil::GS_resizeForBitmap(int resize_height, int resize_width, cv::Mat image)
{
	// (����!) BITMAP�� Ư�� : ������ ���̰� 4�� ����� �ƴϸ� ��׷���
	int height = image.rows;
	int width = image.cols - (image.rows % 4);

	int sHeight;
	int sWidth;

	if (height > resize_height)
	{
		sHeight = resize_height;
		width = (int)(width*(resize_height / (double)height));
		sWidth = width - (width % 4);
	}
	else if (width > resize_width)
	{
		sHeight = height;
		sWidth = resize_width - (resize_width % 4);
	}
	else
	{
		sHeight = height;
		sWidth = width;
	}

	// sHeight x sWidth ũ���� ���� ����
	cv::Mat resize_image = cv::Mat(cv::Size(sWidth, sHeight), image.type());

	// sHeight x sWidth ũ��� Ȯ��/��� �Ѵ�.
	cv::resize(image, resize_image, resize_image.size(), 0, 0, cv::INTER_CUBIC);

	return resize_image;
}

void CUtil::GS_viewMatImage(char *title, cv::Mat image)
{
	// Window�� �Ӽ��� ����
	cv::namedWindow(title, cv::WINDOW_AUTOSIZE);
	// Window�� Mat ���
	cv::imshow(title, image);

	cv::waitKey(0);
	cv::destroyWindow(title);
}

// grayscale ����
int CUtil::GS_clamping(int var)
{
	int retVal = -1;

	// saturation ��� ����
	if (var > 255) retVal = 255;
	else if (var < 0) retVal = 0;
	else retVal = var;

	return retVal;
}

int CUtil::GS_clamping(float var)
{
	return GS_clamping((int)var);
}

int CUtil::GS_clamping(double var)
{
	return GS_clamping((int)var);
}




// double Ÿ���� ��ϵ� ������ 2���� �迭�� cv::Mat���� ��ȯ�Ѵ�.
cv::Mat CUtil::GS_gray2Mat(double **gray, int height, int width)
{
	cv::Mat dst_image = GS_createImage(cv::Size(width, height), 1);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			dst_image.at<uchar>(i, j) = gray[i][j];
		}
	}

	return dst_image;
}

// double Ÿ���� �÷� ������ 2���� �迭�� IplImage�� ��ȯ�Ѵ�.
cv::Mat CUtil::GS_color2Mat(double ** image1, double ** image2, double ** image3, int height, int width)
{
	cv::Mat dst_image = GS_createImage(cv::Size(width, height), 3);

	cv::Mat mat_image[3];
	mat_image[0] = GS_createImage(cv::Size(width, height), 1);
	mat_image[1] = GS_createImage(cv::Size(width, height), 1);
	mat_image[2] = GS_createImage(cv::Size(width, height), 1);

	for (int i = 0; i<height; i++)
	{
		for (int j = 0; j<width; j++)
		{
			mat_image[0].at<uchar>(i, j) = image1[i][j];
			mat_image[1].at<uchar>(i, j) = image2[i][j];
			mat_image[2].at<uchar>(i, j) = image3[i][j];
		}
	}

	// ����
	cv::merge(mat_image, 3, dst_image);

	// �Ҵ��� �޸� ���� 
	mat_image->release();

	return dst_image;
}

