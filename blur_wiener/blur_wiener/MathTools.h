#pragma once

#include "opencv2/core/core.hpp"

class MathTools
{
public:
	MathTools();
	~MathTools();

	static void wienerFilter(cv::Mat& inputFrame, cv::Mat& blurKernel, double k, cv::Mat& outputFrame);
};

