#pragma once
#include "opencv2\core/core.hpp"
#include "Degrader.h"
using namespace cv;

class GradientOptimizer
{
public:
	GradientOptimizer(Degrader& degrader);
	~GradientOptimizer(void);
	void fillGradient(Mat& grad, Mat& point);
	void optimize(Mat& initValue, double eps);

	double alpha;
	Mat estimate;
	Degrader& gradDegrader;
	

};

