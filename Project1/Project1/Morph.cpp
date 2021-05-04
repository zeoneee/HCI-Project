// Morph.cpp: implementation of the CMorphology class.
//
//////////////////////////////////////////////////////////////////////


#include "Morph.h"
#include "Pixel.h"
#include "Util.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMorph::CMorph()
{

}

CMorph::~CMorph()
{

}

// �������� ������ ����
cv::Mat CMorph::GS_smoothing(cv::Mat src_image, int type, int method)
{
	CPixel cPixel;

	if (src_image.channels() != 1) return cv::Mat();
	
	// ���� ���� ����
	cv::Mat opening_image = GS_opening(src_image, type, method);
	
	// ���� ���� ����
	cv::Mat dst_image = GS_closing(opening_image, type, method);

	// �Ҵ��� �޸� ����
	opening_image.release();

	return dst_image;
}

// �������� �׷����Ʈ ����
cv::Mat CMorph::GS_gradient(cv::Mat src_image, int type, int method)
{
	CPixel cPixel;
	CUtil cUtil;
	cv::Mat tmp_image;

	// �ʱ�ȭ 
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels() != 1) return dst_image;
	cv::Mat dilate_image = cUtil.GS_createImage(src_image.size(), src_image.channels());

	// ���� �������� ��ȯ
	if (method == 0)
	{
		double threshold = 255.0;
		double max_value = 255.0;
		int threshold_type = cv::THRESH_OTSU;

		// otsu �˰����� �̿��� ���� ���� ��ȯ
		cv::Mat binary_image = cPixel.GS_threshold(src_image, threshold,
			max_value, threshold_type);
		tmp_image = binary_image.clone();

		binary_image.release();
	}
	// ��ϵ� �����̸� ����
	else if (method == 1)
	{
		tmp_image = src_image.clone();
	}

	// ����ȭ ��� ����
	cv::Mat kernel = cv::getStructuringElement(type, cv::Size(3, 3), cv::Point(1, 1));
	// �������� �׷����Ʈ ����
	cv::morphologyEx(tmp_image, dst_image, cv::MORPH_GRADIENT, kernel);

	/*
	// cvMorphologyEx() �Լ��� ������� �ʰ� ���� ����.

	// ��â ���� ����
	IplImage *dilate_image = GS_dilate( src_image, type, method );

	// ħ�� ���� ����
	IplImage *erode_image = GS_erode( src_image, type, method );

	// ��â ����� ħ�� ���� ���� ����� �� ����
	IplImage *dst_image = cPixel.GS_subtract_image( dilate_image, erode_image );

	// �Ҵ��� �޸� ����
	cvReleaseImage( &erode_image );
	*/

	// �Ҵ��� �޸� ����
	dilate_image.release();
	tmp_image.release();
	kernel.release();

	return dst_image;
}

// �� ���� �� ��ϴ�� ����
cv::Mat CMorph::GS_well_contrast(cv::Mat src_image, int type)
{
	CPixel cPixel;
	CUtil cUtil;
	int i, j, ret_var;
	double var, tophat_var, well_var;

	// �ʱ�ȭ 
	int height = src_image.rows;
	int width = src_image.cols;
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels() != 1) return dst_image;

	// ž-�� ����
	cv::Mat tophat_image = GS_topHat(src_image, type);

	// �� ���� 
	cv::Mat well_image = GS_well(src_image, type);

	for (i = 0; i<height; i++)
	{
		for (j = 0; j<width; j++)
		{
			
			var = src_image.at<float>(i, j); // float? uchar? lets figure it out.
			tophat_var = tophat_image.at<float>(i, j);// float? uchar? lets figure it out.
			well_var = well_image.at<float>(i, j);// float? uchar? lets figure it out.

			ret_var = cUtil.GS_clamping((var + tophat_var) - well_var);

			//cvSetReal2D(dst_image, i, j, ret_var);
			dst_image.at<float>(i, j) = ret_var;
		}
	}

	// �Ҵ��� �޸� ����
	tophat_image.release();

	return dst_image;
}

// �� ����
cv::Mat CMorph::GS_well(cv::Mat src_image, int type)
{
	CPixel cPixel;
	CUtil cUtil;

	// �ʱ�ȭ 
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels() != 1) return dst_image;
	cv::Mat closing_image = cUtil.GS_createImage(src_image.size(), src_image.channels());

	// ����ȭ ��� ����
	cv::Mat kernel = cv::getStructuringElement(type, cv::Size(3, 3), cv::Point(1, 1));

	// �� ����
	cv::morphologyEx(src_image, dst_image, cv::MORPH_BLACKHAT, kernel);

	/*
	// cvMorphologyEx() �Լ��� ������� �ʰ� ���� ����.

	// �� ��ϵ� ���� ���� ���� ���� ����
	IplImage *closing_image = GS_closing( src_image, type, 1 );

	// �ʱ�ȭ
	IplImage *dst_image = cUtil.GS_createImage( cvGetSize(src_image), src_image->nChannels );

	// �� ��ϵ� ����� ���� ���� ���� ������ �� ����
	double ret_var;
	for(int i=0; i<src_image->height; i++)
	{
	for(int j=0; j<src_image->width; j++)
	{
	// �� ����
	ret_var = cvGetReal2D( src_image, i, j ) - cvGetReal2D( closing_image, i, j );

	// ���밪 ���� �� Ŭ����
	ret_var = cUtil.GS_clamping( fabs(ret_var) );

	cvSetReal2D( dst_image, i, j, ret_var );
	}
	}
	*/
	// �Ҵ��� �޸� ����
	closing_image.release();
	kernel.release();

	return dst_image;
}

// ž-�� ���� �� ��ϴ�� ����
cv::Mat CMorph::GS_topHat_contrast(cv::Mat src_image, int type, double contrast, int brightness)
{
	CPixel cPixel;

	if (src_image.channels() != 1) return cv::Mat(); // NULL�� ��� return �Ұ��ΰ�

	// ž-�� ����
	cv::Mat tophat_image = GS_topHat(src_image, type);

	// ��� ���� + ��ϴ�� ����
	cv::Mat dst_image
		= cPixel.GS_LUT_basic_contrast_brightness(tophat_image, contrast, brightness);

	// �Ҵ��� �޸� ����
	tophat_image.release();

	return dst_image;
}

// ž-�� ����
cv::Mat CMorph::GS_topHat(cv::Mat src_image, int type)
{
	CUtil cUtil;

	// �ʱ�ȭ 
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels()!= 1) return dst_image;
	cv::Mat opening_image = cUtil.GS_createImage(src_image.size(), src_image.channels());

	// ����ȭ ��� ����
//    cv::Mat kernel = cv::getStructuringElement(type, cv::Size(3, 3), cv::Point(1, 1));
    cv::Mat kernel = cv::getStructuringElement(type, cv::Size(11, 11), cv::Point());

	// ž-�� ����
	cv::morphologyEx(src_image, dst_image, cv::MORPH_TOPHAT, kernel);

	/*
	// cvMorphologyEx() �Լ��� ������� �ʰ� ���� ����.

	CPixel cPixel;

	// �� ��ϵ� ���� ���� ���� ���� ����
	IplImage *opening_image = GS_opening( src_image, type, 1 );

	// �� ��ϵ� ����� ���� ���� ���� ������ �� ����
	IplImage *dst_image = cPixel.GS_subtract_image( src_image, opening_image );

	*/

	// �Ҵ��� �޸� ����
	opening_image.release();
	kernel.release();

	return dst_image;
}

// ���� ����
cv::Mat CMorph::GS_closing(cv::Mat src_image, int type, int method)
{
	CPixel cPixel;
	CUtil cUtil;
	cv::Mat tmp_image;

	// �ʱ�ȭ 
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels() != 1) return dst_image;
	cv::Mat dilate_image = cUtil.GS_createImage(src_image.size(), src_image.channels());

	// ���� �������� ��ȯ
	if (method == 0)
	{
		double threshold = 255.0;
		double max_value = 255.0;
		int threshold_type = cv::THRESH_OTSU;

		// otsu �˰����� �̿��� ���� ���� ��ȯ
		cv::Mat binary_image = cPixel.GS_threshold(src_image, threshold,
			max_value, threshold_type);
		tmp_image = binary_image.clone();

		binary_image.release();
	}
	// ��ϵ� �����̸� ����
	else if (method == 1)
	{
		tmp_image = src_image.clone();
	}

	// ����ȭ ��� ����
	/* Type  = cv::Morphshape
	   MORPH_RECT / MORPH_ELLIPSE / MORPH_CROSS */
	cv::Mat kernel = cv::getStructuringElement(type, cv::Size(3, 3), cv::Point(1, 1));
	
	// ���� ���� src dst temp element operation
    cv::morphologyEx(tmp_image, dst_image, cv::MORPH_CLOSE, kernel);
	/*
	// cvMorphologyEx() �Լ��� ������� �ʰ� ���� ����.
	// ��â ����
	cvDilate( tmp_image, dilate_image, element, 1 );

	// ħ�� ����
	cvErode( dilate_image, dst_image, element, 1 );
	*/

	// �Ҵ��� �޸� ����
	dilate_image.release();
	tmp_image.release();
	kernel.release();

	return dst_image;
}

// ���� ����
cv::Mat CMorph::GS_opening(cv::Mat src_image, int type, int method)
{
	CPixel cPixel;
	CUtil cUtil;
	cv::Mat tmp_image;

	// �ʱ�ȭ 
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels() != 1) return dst_image;
	cv::Mat erode_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	
	// ���� �������� ��ȯ
	if (method == 0)
	{
		double threshold = 255.0;
		double max_value = 255.0;
		int threshold_type = cv::THRESH_OTSU;

		// otsu �˰����� �̿��� ���� ���� ��ȯ
		cv::Mat binary_image = cPixel.GS_threshold(src_image, threshold,
			max_value, threshold_type);
		tmp_image = binary_image.clone();

		binary_image.release();
	}
	// ��ϵ� �����̸� ����
	else if (method == 1)
	{
		tmp_image = src_image.clone();
	}

	// ����ȭ ��� ����
	cv::Mat kernel = cv::getStructuringElement(type, cv::Size(3, 3), cv::Point(1, 1));

	// ���� ���� morphologyEX(source, destination, operation, kernel)
    cv::morphologyEx(tmp_image, dst_image, cv::MORPH_OPEN, kernel);

	/*
	// cvMorphologyEx() �Լ��� ������� �ʰ� ���� ����.
	// ħ�� ����
	cvErode( tmp_image, erode_image, element );

	// ��â ����
	cvDilate( erode_image, dst_image, element );
	*/

	// �Ҵ��� �޸� ����
	erode_image.release();
	tmp_image.release();
	kernel.release();

	return dst_image;
}

// ��â ����

cv::Mat CMorph::GS_dilate(cv::Mat src_image, int type, int method)
{
	CPixel cPixel;
	CUtil cUtil;
	cv::Mat tmp_image;

	// �ʱ�ȭ 
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels() != 1) return dst_image;

	// ���� �������� ��ȯ
	if (method == 0)
	{
		double threshold = 255.0;
		double max_value = 255.0;
		int threshold_type = cv::THRESH_OTSU;

		// otsu �˰����� �̿��� ���� ���� ��ȯ
		cv::Mat binary_image = cPixel.GS_threshold(src_image, threshold,
			max_value, threshold_type);
		// ���� ���� ��ȯ ��� ���� 
		tmp_image = binary_image.clone();

		binary_image.release();
	}
	// ��ϵ� �����̸� ����
	else if (method == 1)
	{
		tmp_image = src_image.clone();
	}

	// ����ȭ ��� ����
	cv::Mat kernel = cv::getStructuringElement(type, cv::Size(3, 3), cv::Point(1, 1));

	// ��â ����
	cv::dilate(tmp_image, dst_image, kernel);

	// �Ҵ��� �޸� ����
	tmp_image.release();
	kernel.release();

	return dst_image;
}

// ħ�� ����
cv::Mat CMorph::GS_erode(cv::Mat src_image, int type, int method)
{
	CPixel cPixel;
	CUtil cUtil;
	cv::Mat tmp_image;

	// �ʱ�ȭ 
	cv::Mat dst_image = cUtil.GS_createImage(src_image.size(), src_image.channels());
	if (dst_image.channels() != 1) return dst_image;

	// ���� �������� ��ȯ
	if (method == 0)
	{
		double threshold = 255.0;
		double max_value = 255.0;
		int threshold_type = cv::THRESH_OTSU;

		// otsu �˰����� �̿��� ���� ���� ��ȯ
		cv::Mat binary_image = cPixel.GS_threshold(src_image, threshold,
			max_value, threshold_type);

		// ���� ���� ��ȯ ��� ���� 
		tmp_image = binary_image.clone();

		binary_image.release();
	}
	// ��ϵ� �����̸� ����
	else if (method == 1)
	{
		tmp_image = src_image.clone();
	}

	// ����ȭ ��� ����
	cv::Mat kernel = cv::getStructuringElement(type, cv::Size(3, 3), cv::Point(1, 1));

	// ħ�� ����
	cv::erode(tmp_image, dst_image, kernel);

	// �Ҵ��� �޸� ����
	tmp_image.release();
	kernel.release();

	return dst_image;
}
