#pragma once
#include "degrader.h"
class GaussianDegrader :
	public Degrader
{
public:
	GaussianDegrader(void);
	GaussianDegrader(double sigma);
	~GaussianDegrader(void);
	void process(Mat& src, Mat& dst) override;

	Mat kernel;
private:
	void degradeImage(Mat& src, Mat& result, Mat& blurKernel);
};

