#include "GradientOptimizer.h"


GradientOptimizer::GradientOptimizer(Degrader& degrader) : gradDegrader(degrader)
{
	//Needs to be changed in the future and passed as a parameter.
	alpha = 0.01;	
}


GradientOptimizer::~GradientOptimizer(void)
{
}

void GradientOptimizer::fillGradient(Mat& grad, Mat& point)
{
	gradDegrader.process(point, grad);
}

void GradientOptimizer::optimize(Mat& initValueU, double termEps)
{
	initValueU.convertTo(estimate, CV_32F); 	
	double currentGradNorm = 10e5;
	Mat grad = estimate.clone();
	while (currentGradNorm > termEps)
	{
		fillGradient(grad, estimate);
		estimate = estimate - alpha*grad;
		currentGradNorm = norm(grad);
	}
	estimate.convertTo(initValueU, CV_8U);
}

