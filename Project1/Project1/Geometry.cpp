// Geometry.cpp: implementation of the CGeometry class.
//
//////////////////////////////////////////////////////////////////////

#include "Geometry.h"
#include "Pixel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#define cvWarpPerspectiveQMatrix cvGetPerspectiveTransform
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGeometry::CGeometry()
{

}

CGeometry::~CGeometry()
{

}


// ���� ���� ���� (extraction the region of interest)
// ���� ������ return��.
// Mat(Rect(int x,int y,int width,int height)) �� ����
cv::Mat CGeometry::GS_getROI(cv::Mat src_image, int startH, int startW, int height, int width)
{
	cv::Rect roi;
	roi.y = startH; // ���̿� ���� ���� ��ġ (Starting point for height)
	roi.x = startW; // �ʺ� ���� ���� ��ġ (Starting point for width)
	roi.height = height; // ������ ���̿� ���� ���� (
	roi.width = width; // ������ �ʺ� ���� ����

	cv::Mat	dst_image = src_image(roi);

	return dst_image;
}

// ���� ���� ����� ���� ������� ����  (restoration of the region of interest that was extracted)
// cv::Mat �ڷ����� �ش� ��� ����.
//void CGeometry::GS_resetROI(IplImage *roi_image)
//{
//    cvResetImageROI(roi_image);
//}


// �ݻ� (Reflection) or ��Ī
cv::Mat CGeometry::GS_reflect(cv::Mat src_image, int flip_mode)
{
	cv::Mat dst_image = cv::Mat(src_image.size(), src_image.type(), cv::Scalar(0));

	// flip_mode = 0 : ���� ������(flipping)
	// flip_mode > 0 : �¿� ������(mirroring)
	// flip_mode < 0 : ���� ������, �¿� ������ �� ����(flipping + mirroring)
	flip(src_image, dst_image, flip_mode);
	
	return dst_image;
}

// ���� ��ȯ (Perspective transform)
cv::Mat CGeometry::GS_perspective(cv::Mat src_image, cv::Point2f src_point, cv::Point2f dst_point)
{
	// �ʱ�ȭ (Initialization)
	cv::Mat dst_image = cv::Mat(src_image.size(), src_image.type(), cv::Scalar(0));

	cv::Mat map_matrix = cv::Mat(3, 3, CV_32FC1);

	cv::getPerspectiveTransform((cv::InputArray)src_point, (cv::InputArray)dst_point);
	cv::warpPerspective(src_image, dst_image, map_matrix, dst_image.size());

	// �Ҵ��� �޸� ���� (memory deallocation)
	map_matrix.release();

	return dst_image;
}

// ������ ��ȯ (Affine transform)
cv::Mat CGeometry::GS_affine(cv::Mat src_image, cv::Mat map_matrix)
{
	// �ʱ�ȭ (Initialization)
	cv::Mat dst_image = cv::Mat(src_image.size(), src_image.type(), cv::Scalar(0));

	warpAffine(src_image, dst_image, map_matrix, dst_image.size(), cv::INTER_LINEAR + cv::WARP_FILL_OUTLIERS);

	return dst_image;
}

// ������ ��ȯ�� �̿��� ȸ�� (Rotation using affine transform)
cv::Mat CGeometry::GS_affineRotate(cv::Mat src_image, double angle, double scale)
{
	// �ʱ�ȭ (Initialization)
	int height = src_image.rows;
	int width = src_image.cols;
	cv::Mat dst_image = cv::Mat(src_image.size(), src_image.type(), cv::Scalar(0));

	// ȸ�� �߽��� (center point of rotation)
	cv::Point2f center(width / 2.0, height / 2.0);
	
	cv::Mat map_matrix = cv::Mat(2, 3, CV_32FC1);

	// angle : ȸ�� ����
	// scale : Ȯ��/��� ���� , ���� �Ǽ�������.
	map_matrix = cv::getRotationMatrix2D(center, angle, scale);
	
	warpAffine(src_image, dst_image, map_matrix, dst_image.size(), cv::INTER_LINEAR + cv::WARP_FILL_OUTLIERS);

	// �Ҵ��� �޸� ���� (memory deallocation)
	map_matrix.release();

	return dst_image;
}

