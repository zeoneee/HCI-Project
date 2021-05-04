// Edge.cpp: implementation of the CEdge class.
//
//////////////////////////////////////////////////////////////////////

#include "Edge.h"
#include "Util.h"
#include "Filter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEdge::CEdge()
{

}

CEdge::~CEdge()
{

}

// ĳ�� ���� ���� : Canny() �Լ� �̿�  (Extraction canny edge : using cv::Canny())
cv::Mat CEdge::GS_canny_edge_Canny(cv::Mat src_image, double low_threshold, double high_threshold)
{
	CUtil cUtil;

	// �ʱ�ȭ (Initialization)
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels() != 1) return dst_image;

	// 
	cv::Mat canny_edge = dst_image.clone();

	// (1) Canny() �Լ� ����ô� src_image�� dst_image�� depth�� ���ƾ� ��. (depth of src_image and dst_image shoud be the same in Canny())
	// (2) low_threshold, high_threshold�� Edge linking ó���ϱ� ���� ��.(low_threshold, high_threshold are for Edge linking processing)
	cv::Canny(src_image, canny_edge, low_threshold, high_threshold, 3);
	cv::convertScaleAbs(canny_edge, dst_image, 1, 0);

	return dst_image;
}

// ĳ�� ���� ���� (Extraction canny edg)
cv::Mat CEdge::GS_canny_edge(cv::Mat src_image, double sigma, int threshold, int method)
{
	int i, j;
	double Gx_sum, Gy_sum, ret_var;
	CUtil cUtil;
	CFilter cFilter;

	// �ʱ�ȭ (Initialization)
	int height = src_image.rows;
	int width = src_image.cols;
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels() != 1) return dst_image;

	// ����þ� ������ (Gaussian smoothing)
	int mask_height, mask_width;
	GS_get_LoG_mask(sigma, &mask_height, &mask_width);
	cv::Mat gs_image = cFilter.GS_gaussian_smoothing_filtering(src_image, sigma);

	// ó�� �ð��� ���̱� ���� ����ũ�� ���� �ʰ� ���� ���Ѵ�.
	for (i = 0; i<height - mask_height + ((mask_height - 1) / 2); i++)
	{
		for (j = 0; j < width - mask_width + ((mask_width - 1) / 2); j++)
		{
			// �Һ�
			if (method == 0)
			{
				// ���� ����ũ (vertical mask)
				Gx_sum = -gs_image.at<uchar>(i, j)
					- 2 * gs_image.at<uchar>(i + 1, j)
					- gs_image.at<uchar>(i + 2, j)
					+ gs_image.at<uchar>(i, j + 2)
					+ 2 * gs_image.at<uchar>(i + 1, j + 2)
					+ gs_image.at<uchar>(i + 2, j + 2);
				// ���� ����ũ (horizontal mask)
				Gy_sum = -gs_image.at<uchar>(i, j)
					- 2 * gs_image.at<uchar>(i, j+1)
					- gs_image.at<uchar>(i, j+2)
					+ gs_image.at<uchar>(i+2, j)
					+ 2 * gs_image.at<uchar>(i + 2, j + 1)
					+ gs_image.at<uchar>(i + 2, j + 2);
			}
			// ������
			else if (method == 1)
			{
				// ���� ����ũ (vertical mask)
				Gx_sum = -gs_image.at<uchar>(i, j)
					- gs_image.at<uchar>(i + 1, j)
					- gs_image.at<uchar>(i + 2, j)
					+ gs_image.at<uchar>(i, j + 2)
					+ gs_image.at<uchar>(i + 1, j + 2)
					+ gs_image.at<uchar>(i + 2, j + 2);
				// ���� ����ũ (horizontal mask)
				Gy_sum = -gs_image.at<uchar>(i, j)
					- gs_image.at<uchar>(i, j + 1)
					- gs_image.at<uchar>(i, j + 2)
					+ gs_image.at<uchar>(i + 2, j)
					+ gs_image.at<uchar>(i + 2, j + 1)
					+ gs_image.at<uchar>(i + 2, j + 2);
			}
			// �ι���
			else if (method == 2)
			{
				// �밢�� ���� (diagonal direction)
				Gx_sum = -gs_image.at<uchar>(i, j + 2) + gs_image.at<uchar>(i + 1, j + 1);

				// ���밢�� ���� (reverse diagonal direction)
				Gy_sum = -gs_image.at<uchar>(i, j) + gs_image.at<uchar>(i + 1, j + 1);
			}

			ret_var = fabs(Gx_sum) + fabs(Gy_sum);

			// hysteresis ����
			// ��, lower, upper ��谪 ó�� �ϴ� ��.(lower, upper boundary value)
			// ���⼭ upper�� 255�� ���Ͽ���. (upper==255)
			if (ret_var > 255.0) ret_var = 0.0;
			else if (ret_var < (double)threshold) ret_var = 0.0;

			dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = ret_var;
		}
	}

	return dst_image;
}
// LoG(Laplacian of Gaussian) ���� ���� : ���� (Extraction LoG(Laplacian of Gaussian) edge: formula)


double **CEdge::GS_get_LoG_mask(double sigma, int *mask_height, int *mask_width)
{
	int i, j;
	double tmp;

	// ����ũ�� ũ�⸦ �����Ѵ�. (set the size of mask)
	int size = (int)(3.35*sigma + 0.33);
	int mask_length = 2 * size + 1;

	CUtil cUtil;
	double **mask = cUtil.GS_doubleAlloc2D(mask_length, mask_length);

	for (i = 0; i<mask_length; i++)
	{
		for (j = 0; j<mask_length; j++)
		{
			tmp = GS_get_distance((double)i, (double)j, (double)size, (double)size);
			mask[i][j] = GS_get_LoG_formular(tmp, sigma);
		}
	}

	*mask_height = *mask_width = mask_length;
	return mask;
}

double CEdge::GS_get_LoG_formular(double x, double sigma)
{
	return (x*x - 2.0*sigma*sigma) / (sigma*sigma*sigma*sigma) * gauss(x, sigma);
}

double CEdge::norm(double x, double y)
{
	return sqrt(x*x + y*y);
}

double CEdge::GS_get_distance(double a, double b, double c, double d)
{
	return norm((a - c), (b - d));
}

double CEdge::gauss(double x, double sigma)
{
	return exp((-x*x) / (2.0*sigma*sigma));
}

// LoG(Laplacian of Gaussian) : ����ũ (mask)


// ���ö�þ� ������ (Laplacian Sharpening)
// method - 0 : 4 ���� ����ũ 1 (mask 1 for 4 directions)
//        - 1 : 4 ���� ����ũ 2 (mask 2 for 4 directions)
//        - 2 : 8 ���� ����ũ 1 (mask 1 for 8 directions)
//        - 3 : 8 ���� ����ũ 2 (mask 2 for 8 directions)
cv::Mat CEdge::GS_laplacian_sharpening(cv::Mat src_image, int method)
{
	CUtil cUtil;

	// �ʱ�ȭ (Initialization)
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels() != 1) return dst_image;

	// ���ö�þ� ���� ����(Extraction Laplacian edge)
	cv::Mat laplacian_edge = GS_laplacian_edge(src_image, method);

	// ���� ���� (add)
	cv::add(src_image, laplacian_edge, dst_image);

	// �Ҵ��� �޸� ����(memory deallocation)
	laplacian_edge.release();

	return dst_image;
}

// ���ö�þ� ���� ���� : Laplacian() (Extraction Laplacian edge: Laplacian())
cv::Mat CEdge::GS_laplacian_edge_Laplacian(cv::Mat src_image)
{
	CUtil cUtil;

	// �ʱ�ȭ 
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels() != 1) return dst_image;

	cv::Mat laplacian_edge = cUtil.GS_createImage(src_image.size(), CV_32F, src_image.channels());

	// 4��° ���ڰ� 1�̸� �Ʒ��� ���ö�þ� ����ũ�� ���� ó���Ѵ�. (if the third argument is 1, use Laplacian mask below)
	// |0  1  0|
	// |1 -4  1|
	// |0  1  0|
	// https://docs.opencv.org/3.4.1/d4/d86/group__imgproc__filter.html#gad78703e4c8fe703d479c1860d76429e6
	// 4 ��° ���ڰ� 1�� default�� ����Ǿ��־� �ؿ����� ������.
	cv::Laplacian(src_image, laplacian_edge, laplacian_edge.depth());

	laplacian_edge.convertTo(dst_image, CV_8UC1, 1, 0);
	
	return dst_image;
}

// ���ö�þ� ���� ���� : ����ũ (Extraction Laplacian edge: mask)
// method - 0 : 4 ���� ����ũ 1 (mask 1 for 4 directions)
//        - 1 : 4 ���� ����ũ 2 (mask 2 for 4 directions)
//        - 2 : 8 ���� ����ũ 1 (mask 1 for 8 directions)
//        - 3 : 8 ���� ����ũ 2 (mask 2 for 8 directions)
cv::Mat CEdge::GS_laplacian_edge(cv::Mat src_image, int method)
{
	int i, j, m, n;
	int mask_height, mask_width;
	double var, ret_var;
	CUtil cUtil;

	// �ʱ�ȭ  (Initialization)
	int height = src_image.rows;
	int width = src_image.cols;
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels() != 1) return dst_image;

	// ���ö�þ� ����ũ ����  (define Laplacian mask)
	mask_height = mask_width = 3;
	int mask_four1[3][3] = { { 0, -1, 0 },{ -1, 4, -1 },{ 0, -1, 0 } };
	int mask_four2[3][3] = { { 0, 1, 0 },{ 1, -4, 1 },{ 0, 1, 0 } };
	int mask_eight1[3][3] = { { -1, -1, -1 },{ -1, 8, -1 },{ -1, -1, -1 } };
	int mask_eight2[3][3] = { { 1, -2, 1 },{ -2, 4, -2 },{ 1, -2, 1 } };

	for (i = 0; i<height - mask_height + ((mask_height - 1) / 2); i++)
	{
		for (j = 0; j<width - mask_width + ((mask_width - 1) / 2); j++)
		{
			// �ʱ�ȭ (Initialization)
			ret_var = 0.0;

			if ((i + mask_height > height) || (j + mask_width > width))
				continue;

			for (m = 0; m<mask_height; m++)
			{
				for (n = 0; n<mask_width; n++)
				{
					var = src_image.at<uchar>(i + m, j + n);

					if (method == 0) ret_var += var * mask_four1[m][n];
					else if (method == 1) ret_var += var * mask_four2[m][n];
					else if (method == 2) ret_var += var * mask_eight1[m][n];
					else if (method == 3) ret_var += var * mask_eight2[m][n];
				}
			}

			ret_var = cUtil.GS_clamping(ret_var);
			dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = ret_var;
		}
	}

	return dst_image;
}

// �ι��� ���� ���� (Extraction Roberts edge)
cv::Mat CEdge::GS_roberts_edge(cv::Mat src_image, int method)
{
	int i, j, m, n;
	int mask_height, mask_width;
	double diagonal_var1, diagonal_var2, ret_var;
	CUtil cUtil;

	// �ʱ�ȭ (Initialization)
	int height = src_image.rows;
	int width = src_image.cols;
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels() != 1) return dst_image;

	// �밢�� ����ũ ���� (define diagonal mask)
	mask_height = mask_width = 3;
	int diagonal_mask1[3][3] = { { -1, 0, 0 },{ 0, 1, 0 },{ 0, 0, 0 }, };
	int diagonal_mask2[3][3] = { { 0, 0, -1 },{ 0, 1, 0 },{ 0, 0, 0 }, };


	for (i = 0; i<height - mask_height + ((mask_height - 1) / 2); i++)
	{
		for (j = 0; j<width - mask_width + ((mask_width - 1) / 2); j++)
		{
			// �ʱ�ȭ (Initialization)
			diagonal_var1 = diagonal_var2 = 0.0;

			if ((i + mask_height > height) || (j + mask_width > width))
				continue;

			for (m = 0; m<mask_height; m++)
			{
				for (n = 0; n<mask_width; n++)
				{
					// �밢�� ���� 1 (diagonal edge1)
					diagonal_var1 += src_image.at<uchar>(i + m, j + n) * diagonal_mask1[m][n];

					// �밢�� ���� 2 (diagonal edge1)
					diagonal_var2 += src_image.at<uchar>(i + m, j + n) * diagonal_mask2[m][n];
				}
			}

			diagonal_var1 = fabs(diagonal_var1);
			diagonal_var2 = fabs(diagonal_var2);
			ret_var = diagonal_var1 + diagonal_var2;

			if (method == 0)
			{
				dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = diagonal_var1;
			}
			else if (method == 1)
			{
				dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = diagonal_var2;
			}
			else if (method == 2)
			{
				dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = ret_var;
			}
		}
	}

	return dst_image;
}

// ����ĳ��ƽ ���� ���� (Extraction Stochastic edge)
cv::Mat CEdge::GS_stochastic_Edge(cv::Mat src_image, int method)
{
	int i, j, m, n;
	int mask_height, mask_width;
	double vertical_var, horizontal_var, ret_var;
	CUtil cUtil;

	// �ʱ�ȭ (Initialization)
	int height = src_image.rows;
	int width = src_image.cols;
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels() != 1) return dst_image;

	// ����/���� ����ũ ���� (define vertical/horizontal mask)
	mask_height = mask_width = 3;
	double vertical_mask[3][3] = { { -0.97, 0, 0.97 },{ -1, 0, 1 },{ -0.97, 0, 0.97 } };
	double horizontal_mask[3][3] = { { 0.97, 1, 0.97 },{ 0, 0, 0 },{ -0.97, -1, -0.97 } };

	for (i = 0; i<height - mask_height + ((mask_height - 1) / 2); i++)
	{
		for (j = 0; j<width - mask_width + ((mask_width - 1) / 2); j++)
		{
			// �ʱ�ȭ (Initialization)
			vertical_var = horizontal_var = 0.0;

			if ((i + mask_height > height) || (j + mask_width > width))
				continue;

			for (m = 0; m<mask_height; m++)
			{
				for (n = 0; n<mask_width; n++)
				{
					// ���� ���� (vertical edge)
					vertical_var += src_image.at<uchar>(i + m, j + n) * vertical_mask[m][n];

					// ���� ���� (horizontal edge)
					horizontal_var += src_image.at<uchar>(i + m, j + n)*horizontal_mask[m][n];
				}
			}

			vertical_var = fabs(vertical_var);
			horizontal_var = fabs(horizontal_var);
			ret_var = vertical_var + horizontal_var;

			if (method == 0)
			{
				dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = vertical_var;
			}
			else if (method == 1)
			{
				dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = horizontal_var;
			}
			else if (method == 2)
			{
				dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = ret_var;
			}
		}
	}

	return dst_image;
}

// ������þ ���� ���� (Extraction freichen edge)
cv::Mat CEdge::GS_freichen_edge(cv::Mat src_image, int method)
{
	int i, j, m, n;
	int mask_height, mask_width;
	double vertical_var, horizontal_var, ret_var;
	CUtil cUtil;

	// �ʱ�ȭ (Initialization)
	int height = src_image.rows;
	int width = src_image.cols;
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels() != 1) return dst_image;

	// ����/���� ����ũ ���� (define vertical/horizontal mask)
	mask_height = mask_width = 3;
	double vertical_mask[3][3] = { { -1, 0, 1 },{ -sqrt(2), 0, sqrt(2) },{ -1, 0, 1 } };
	double horizontal_mask[3][3] = { { 1, sqrt(2), 1 },{ 0, 0, 0 },{ -1, -sqrt(2), -1 } };

	for (i = 0; i<height - mask_height + ((mask_height - 1) / 2); i++)
	{
		for (j = 0; j<width - mask_width + ((mask_width - 1) / 2); j++)
		{
			// �ʱ�ȭ (Initialization)
			vertical_var = horizontal_var = 0.0;

			if ((i + mask_height > height) || (j + mask_width > width))
				continue;

			for (m = 0; m<mask_height; m++)
			{
				for (n = 0; n<mask_width; n++)
				{
					// ���� ���� (vertical edge)
					vertical_var += src_image.at<uchar>(i + m, j + n)*vertical_mask[m][n];

					// ���� ���� (horizontal edge)
					horizontal_var += src_image.at<uchar>(i + m, j + n) * horizontal_mask[m][n];
				}
			}

			vertical_var = fabs(vertical_var);
			horizontal_var = fabs(horizontal_var);
			ret_var = vertical_var + horizontal_var;

			if (method == 0)
			{
				dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = vertical_var;
			}
			else if (method == 1)
			{
				dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = horizontal_var;
			}
			else if (method == 2)
			{
				dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = ret_var;
			}
		}
	}

	return dst_image;
}


// ������ ���� ���� (Extraction prewitt edge)
cv::Mat CEdge::GS_prewitt_edge(cv::Mat src_image, int method)
{
	int i, j, m, n;
	int mask_height, mask_width;
	double vertical_var, horizontal_var, ret_var;
	CUtil cUtil;

	// �ʱ�ȭ (Initialization)
	int height = src_image.rows;
	int width = src_image.cols;
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels() != 1) return dst_image;

	// ����/���� ����ũ ���� (define vertical/horizontal mask)
	mask_height = mask_width = 3;
	int vertical_mask[3][3] = { { -1, 0, 1 },{ -1, 0, 1 },{ -1, 0, 1 } };
	int horizontal_mask[3][3] = { { 1, 1, 1 },{ 0, 0, 0 },{ -1, -1, -1 } };

	for (i = 0; i<height - mask_height + ((mask_height - 1) / 2); i++)
	{
		for (j = 0; j<width - mask_width + ((mask_width - 1) / 2); j++)
		{
			// �ʱ�ȭ (Initialization)
			vertical_var = horizontal_var = 0.0;

			if ((i + mask_height > height) || (j + mask_width > width))
				continue;

			for (m = 0; m<mask_height; m++)
			{
				for (n = 0; n<mask_width; n++)
				{
					// ���� ���� (vertical edge)
					vertical_var += src_image.at<uchar>(i + m, j + n) * vertical_mask[m][n];

					// ���� ���� (horizontal edge)
					horizontal_var += src_image.at<uchar>(i + m, j + n) * horizontal_mask[m][n];
				}
			}

			vertical_var = fabs(vertical_var);
			horizontal_var = fabs(horizontal_var);
			ret_var = vertical_var + horizontal_var;

			if (method == 0)
			{
				dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = vertical_var;
			}
			else if (method == 1)
			{
				dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = horizontal_var;
			}
			else if (method == 2)
			{
				dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = ret_var;
			}
		}
	}

	return dst_image;
}

// �Һ� ���� ���� : �밢�� (Extraction sobel edge: diagonal)
cv::Mat CEdge::GS_sobel_diagonal_edge(cv::Mat src_image, int method)
{
	int i, j, m, n;
	int mask_height, mask_width;
	double diagonal_var1, diagonal_var2, ret_var;
	CUtil cUtil;

	// �ʱ�ȭ (Initialization)
	int height = src_image.rows;
	int width = src_image.cols;
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels() != 1) return dst_image;

	// ����/���� ����ũ ���� (define vertical/horizontal mask)
	mask_height = mask_width = 3;
	int diagonal_mask1[3][3] = { { -2, -1, 0 },{ -1, 0, 1 },{ 0, 1, 2 } };
	int diagonal_mask2[3][3] = { { 0, -1, -2 },{ 1, 0, -1 },{ 2, 1, 0 } };

	for (i = 0; i<height - mask_height + ((mask_height - 1) / 2); i++)
	{
		for (j = 0; j<width - mask_width + ((mask_width - 1) / 2); j++)
		{
			// �ʱ�ȭ (Initialization)
			diagonal_var1 = diagonal_var2 = 0.0;

			if ((i + mask_height > height) || (j + mask_width > width))
				continue;

			for (m = 0; m<mask_height; m++)
			{
				for (n = 0; n<mask_width; n++)
				{
					// �밢�� ���� 1 (diagonal edge1)
					diagonal_var1 += src_image.at<uchar>(i + m, j + n) * diagonal_mask1[m][n];

					// �밢�� ���� 2 (diagonal edge2)
					diagonal_var2 += src_image.at<uchar>(i + m, j + n)*diagonal_mask2[m][n];
				}
			}

			diagonal_var1 = fabs(diagonal_var1);
			diagonal_var2 = fabs(diagonal_var2);
			ret_var = diagonal_var1 + diagonal_var2;

			if (method == 0)
			{
				dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = diagonal_var1;
			}
			else if (method == 1)
			{
				dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = diagonal_var2;
			}
			else if (method == 2)
			{
				dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = ret_var;
			}
		}
	}

	return dst_image;
}

// �Һ� ���� ���� : ����ũ(7x7) (Extraction sobel edge : mask(7x7))
cv::Mat CEdge::GS_sobel_edge_7x7(cv::Mat src_image, int method)
{
	int i, j, m, n;
	int mask_height, mask_width;
	double vertical_var, horizontal_var, ret_var;
	CUtil cUtil;

	// �ʱ�ȭ (Initialization)
	int height = src_image.rows;
	int width = src_image.cols;
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels() != 1) return dst_image;

	// ����/���� ����ũ ���� (define vertical/horizontal mask)
	mask_height = mask_width = 7;
	int vertical_mask[7][7] = { { -1, -1, -1, 0, 1, 1, 1 },
	{ -1, -1, -1, 0, 1, 1, 1 },
	{ -1, -1, -1, 0, 1, 1, 1 },
	{ -2, -2, -2, 0, 2, 2, 2 },
	{ -1, -1, -1, 0, 1, 1, 1 },
	{ -1, -1, -1, 0, 1, 1, 1 },
	{ -1, -1, -1, 0, 1, 1, 1 } };

	int horizontal_mask[7][7] = { { 1, 1, 1, 2, 1, 1, 1 },
	{ 1, 1, 1, 2, 1, 1, 1 },
	{ 1, 1, 1, 2, 1, 1, 1 },
	{ 0, 0, 0, 0, 0, 0, 0 },
	{ -1, -1, -1, -2, -1, -1, -1 },
	{ -1, -1, -1, -2, -1, -1, -1 },
	{ -1, -1, -1, -2, -1, -1, -1 } };

	for (i = 0; i<height - mask_height + ((mask_height - 1) / 2); i++)
	{
		for (j = 0; j<width - mask_width + ((mask_width - 1) / 2); j++)
		{
			// �ʱ�ȭ (Initialization)
			vertical_var = horizontal_var = 0.0;

			if ((i + mask_height > height) || (j + mask_width > width))
				continue;

			for (m = 0; m<mask_height; m++)
			{
				for (n = 0; n<mask_width; n++)
				{
					// ���� ���� (vertical edge)
					vertical_var += src_image.at<uchar>(i + m, j + n) * vertical_mask[m][n];

					// ���� ���� (horizontal edge)
					horizontal_var += src_image.at<uchar>(i + m, j + n) * horizontal_mask[m][n];
				}
			}

			vertical_var = fabs(vertical_var);
			horizontal_var = fabs(horizontal_var);
			ret_var = vertical_var + horizontal_var;

			if (method == 0)
			{
				dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = vertical_var;
			}
			else if (method == 1)
			{
				dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = horizontal_var;
			}
			else if (method == 2)
			{
				dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = ret_var;
			}
		}
	}

	return dst_image;
}


// �Һ� ���� ���� : cv::Sobel() �Լ� �̿�(Extraction sobel edge : using cv::Sobel())
cv::Mat CEdge::GS_sobel_edge_Sobel(cv::Mat src_image, int method)
{
	CUtil cUtil;

	// �ʱ�ȭ  (Initialization)
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels() != 1) return dst_image;

	//  
	cv::Mat vertical_edge = cUtil.GS_createImage(src_image.size(), CV_32F, src_image.channels());

	//
	cv::Mat horizontal_edge = cUtil.GS_createImage(src_image.size(), CV_32F, src_image.channels());

	//  cv::Sobel() �Լ� ���� ��° ���ڰ��� 3 �̸� ����þ� ������ ���� 
	// if the third argument is 3, apply Gaussian smoothing
	Sobel(src_image, vertical_edge, -1, 1, 0, 3); // ���� (vertical)
	Sobel(src_image, horizontal_edge, -1, 0, 1, 3); // ���� (horizontal)

	// cv::conertScaleAbs() �Լ��� CV_32F�� CV_8U�� ��ȯ.
	// 0 = ���� ���� 1 = ���� ���� 2 = ����/���� ����
	if (method == 0)
		cv::convertScaleAbs(vertical_edge, dst_image);
	else if (method == 1)
		cv::convertScaleAbs(horizontal_edge, dst_image);
	else if (method == 2)
	{
		cv::Mat tmp_image1 = dst_image.clone();
		cv::Mat tmp_image2 = dst_image.clone();

		cv::convertScaleAbs(vertical_edge, tmp_image1);
		cv::convertScaleAbs(horizontal_edge, tmp_image2);

		cv::add(tmp_image1, tmp_image2, dst_image);

		tmp_image1.release();
		tmp_image2.release();
	}

	vertical_edge.release();
	horizontal_edge.release();

	return dst_image;
}

// �Һ� ���� ���� : ����ũ(3x3) (Extraction sobel edge : mask(3x3))
cv::Mat CEdge::GS_sobel_edge(cv::Mat src_image, int method)
{
	int i, j, m, n;
	int mask_height, mask_width;
	double vertical_var, horizontal_var, ret_var;
	CUtil cUtil;

	// �ʱ�ȭ (Initialization)
	int height = src_image.rows;
	int width = src_image.cols;
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels() != 1) return dst_image;

	// ����/���� ����ũ ���� (define vertical/horizontal mask)
	mask_height = mask_width = 3;
	int vertical_mask[3][3] = { { -1, 0, 1 },{ -2, 0, 2 },{ -1, 0, 1 } };
	int horizontal_mask[3][3] = { { 1, 2, 1 },{ 0, 0, 0 },{ -1, -2, -1 } };

	for (i = 0; i<height - mask_height + ((mask_height - 1) / 2); i++)
	{
		for (j = 0; j<width - mask_width + ((mask_width - 1) / 2); j++)
		{
			// �ʱ�ȭ (Initialization)
			vertical_var = horizontal_var = 0.0;

			if ((i + mask_height > height) || (j + mask_width > width))
				continue;

			for (m = 0; m<mask_height; m++)
			{
				for (n = 0; n<mask_width; n++)
				{
					// ���� ���� (vertical edge)
					vertical_var += src_image.at<uchar>(i + m, j + n) * vertical_mask[m][n];

					// ���� ���� (horizontal edge)
					horizontal_var += src_image.at<uchar>(i + m, j + n) * horizontal_mask[m][n];
				}
			}

			vertical_var = fabs(vertical_var);
			horizontal_var = fabs(horizontal_var);
			ret_var = vertical_var + horizontal_var;

			if (method == 0)
			{
				dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = vertical_var;
			}
			else if (method == 1)
			{
				dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = horizontal_var;
			}
			else if (method == 2)
			{
				dst_image.at<uchar>(i + (mask_height - 1) / 2, j + (mask_width - 1) / 2) = ret_var;
			}
		}
	}

	return dst_image;
}