// Pixel.h: interface for the CPixel class.
//
//////////////////////////////////////////////////////////////////////

#include <opencv2/opencv.hpp>

typedef unsigned char BYTE;

class CPixel  
{
public:
	CPixel();
	virtual ~CPixel();

	// ������ ��� ����
	cv::Mat GS_makeGrayBand();

	// ��� ��� ����
	cv::Mat GS_makeContrast();

	// ��� ���� - ����/����
	cv::Mat GS_add_constant(cv::Mat src_image, int constant );

	// �� ���� ����
	cv::Mat GS_add_image(cv::Mat src_image1, cv::Mat src_image2 );

	// �� ���� ����
	cv::Mat GS_subtract_image(cv::Mat src_image1, cv::Mat src_image2 );

	// ��� ��� ���� - ����
	cv::Mat GS_multiple_constant(cv::Mat src_image, double scale);

	// ��� ��� ���� - ������
	cv::Mat GS_divide_constant(cv::Mat src_image, double scale);

	// ��� ���� + ��� ��� ����
	cv::Mat GS_basic_contrast_brightness(cv::Mat src_image, double contrast, int brightness );

	// ���� ����
	cv::Mat GS_blending_effect(cv::Mat src_image1, cv::Mat src_image2, double alpha );

	// ��ϵ� ������ ������׷� ����.
	cv::Mat GS_imhist(cv::Mat img, int max_length = 256);

	// ������׷� ��Ȱȭ
	cv::Mat GS_histeq(cv::Mat src_image);

	// ������׷� �����͸� �α� â�� ���
	void GS_view_hist_data(cv::Mat src_image);

	// ��� ���̺��� �̿��� ��� ���� + ��� ��� ����
	cv::Mat GS_LUT_basic_contrast_brightness(cv::Mat src_image, double contrast, int brightness );

	// �־��� ���ǿ� ���� ���� ���� ��ȯ
	cv::Mat GS_threshold(cv::Mat src_image, double thresh, int threshold_type);
	cv::Mat GS_threshold(cv::Mat src_image, double thresh, double max_value, int threshold_type);

	// ������ ��谪 ������ ���� ���� ���� ��ȯ
	cv::Mat GS_adaptive_threshold(cv::Mat src_image, double max_value,
									 int adaptive_method, int threshold_type,
									 int block_size = 3, double C = 5);

	// ���� ���󿡼��� �� ���� - �� ����(logical operator) ���
	cv::Mat GS_binary_logic(cv::Mat src_image1, cv::Mat src_image2, int logic_method);

	// ���� ������ ��ϵ� �������� ��ȯ
	cv::Mat GS_binary2gray(cv::Mat src_image);

	// ��ϵ� ���󿡼��� �� ���� - ��Ʈ ������(bitwise operator) ���
	cv::Mat GS_gray_logic(cv::Mat src_image1, cv::Mat src_image2, int logic_method);
};

