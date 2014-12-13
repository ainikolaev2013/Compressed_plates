#pragma once

#include <opencv2\core\core.hpp>

using namespace cv;
class Degrader 
{
public:
	virtual void process(Mat& src, Mat& dst) = 0;
};

