#include "GaussianDegrader.h"
#include "opencv2/imgproc/imgproc.hpp"


GaussianDegrader::GaussianDegrader(void)
{
}


GaussianDegrader::~GaussianDegrader(void)
{
}



GaussianDegrader::GaussianDegrader(double sigma)
{
	int ksize = 6 * sigma;
	if (ksize %2 == 0)
	{
		ksize++;
	}
	Mat l_kernel(ksize, ksize, CV_32F);//getGaussianKernel(i*6+1, i);
	double center = (double) ksize / 2.0 - 0.5;
	for (int rowInd = 0; rowInd < l_kernel.rows; rowInd++)
	{
		for (int colInd = 0; colInd < l_kernel.cols; colInd++)
		{
			double sx = rowInd - center;
			double sy = colInd - center;
			l_kernel.at<float>(rowInd, colInd) = pow(2.7181, (-sx*sx - sy*sy) / 2 / sigma / sigma); 
		}
	}
	Scalar chSum = sum(l_kernel);
	double kSum = chSum[0];
	//std::cout << kSum << std::endl;
	l_kernel = l_kernel / kSum;
		
	kernel = l_kernel;
}

void GaussianDegrader::degradeImage(Mat& src, Mat& result, Mat& blurKernel)
{
	filter2D(src, result, CV_8U, blurKernel);
}

void GaussianDegrader::process(Mat& src, Mat& dst)
{
	degradeImage(src, dst, kernel);
}