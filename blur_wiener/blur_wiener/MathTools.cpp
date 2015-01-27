#include "MathTools.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

MathTools::MathTools()
{
}


MathTools::~MathTools()
{
}

void MathTools::wienerFilter(cv::Mat& inputFrame, cv::Mat& blurKernel, double k, cv::Mat& outputFrame)
{
	cv::Mat padded;                            //expand input image to optimal size
	int m = cv::getOptimalDFTSize(inputFrame.rows);
	int n = cv::getOptimalDFTSize(inputFrame.cols); // on the border add zero values
	cv::copyMakeBorder(inputFrame, padded, 0, m - inputFrame.rows, 0, n - inputFrame.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

	cv::Mat planes[] = { cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F) };
	cv::Mat complexI;
	merge(planes, 2, complexI);         // Add to the expanded another plane with zeros

	cv::dft(complexI, complexI);

	cv::Mat paddedKernel;
	
	cv::copyMakeBorder(blurKernel, paddedKernel, 0, m - blurKernel.rows, 0, n - blurKernel.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

	cv::Mat kernelImg;
	cv::Mat kernelScaled;
	normalize(paddedKernel, kernelScaled, 0, 255, CV_MINMAX);

	kernelScaled.convertTo(kernelImg, CV_8UC1);
	cv::imwrite("kernel2.bmp", kernelImg);

	cv::Mat kernelPlanes[] = { cv::Mat_<float>(paddedKernel), cv::Mat::zeros(paddedKernel.size(), CV_32F) };
	cv::Mat complexKernelI;
	merge(kernelPlanes, 2, complexKernelI);         // Add to the expanded another plane with zeros

	cv::dft(complexKernelI, complexKernelI);

	std::vector<cv::Mat> complexKernelPlanes;
	cv::split(complexKernelI, complexKernelPlanes);
	
	cv::pow(complexKernelPlanes[0], 2.0, complexKernelPlanes[0]);
	cv::pow(complexKernelPlanes[1], 2.0, complexKernelPlanes[1]);
	cv::Mat complexKernelAbsSq;
	cv::add(complexKernelPlanes[0], complexKernelPlanes[1], complexKernelAbsSq);

	cv::add(complexKernelAbsSq, cv::Scalar(k), complexKernelAbsSq);
	
	cv::Mat numerator;
	bool conjB = true;
	cv::mulSpectrums(complexI, complexKernelI, numerator, 0, conjB);	

	std::vector<cv::Mat> numeratorPlanes;
	cv::split(numerator, numeratorPlanes);	

	cv::divide(numeratorPlanes[0], complexKernelAbsSq, numeratorPlanes[0]);
	cv::divide(numeratorPlanes[1], complexKernelAbsSq, numeratorPlanes[1]);
	
	cv::Mat complexResult;
	cv::merge(numeratorPlanes, complexResult);
	cv::dft(complexResult, complexResult, cv::DFT_INVERSE);

	std::vector<cv::Mat> outputFloats;
	cv::split(complexResult, outputFloats);


	cv::Mat outputDiv;
	normalize(outputFloats[0], outputDiv, 0, 255, CV_MINMAX);

	cv::Mat outputFrame1;
	outputDiv.convertTo(outputFrame1, CV_8UC1);
	int shiftX = (blurKernel.cols-1)/2;
	int shiftY = (blurKernel.rows-1)/2;	
	outputFrame.create(outputFrame1.rows, outputFrame1.cols, CV_8UC1);
	outputFrame1(cv::Rect(0, 0, outputFrame.rows - shiftY, outputFrame.cols - shiftX))
		.copyTo(outputFrame(cv::Rect(shiftX, shiftY, outputFrame1.cols - shiftX, outputFrame1.rows - shiftY)));
	outputFrame1(cv::Rect(0, outputFrame1.rows - shiftY, outputFrame.cols - shiftX, shiftY))
		.copyTo(outputFrame(cv::Rect(shiftX, 0, outputFrame1.cols - shiftX, shiftY)));
	outputFrame1(cv::Rect(outputFrame.cols - shiftX, 0, shiftX, outputFrame1.rows - shiftY))
		.copyTo(outputFrame(cv::Rect(0, shiftY, shiftX, outputFrame1.rows - shiftY)));
	outputFrame1(cv::Rect(outputFrame.cols - shiftX, outputFrame1.rows - shiftY, shiftX, shiftY))
		.copyTo(outputFrame(cv::Rect(0, 0, shiftX, shiftY)));

}
