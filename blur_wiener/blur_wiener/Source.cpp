/*
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    if( argc != 2)
    {
     cout <<" Usage: display_image ImageToLoadAndDisplay" << endl;
     return -1;
    }

    Mat image;
    image = imread(argv[1], IMREAD_COLOR); // Read the file

    if(! image.data ) // Check for invalid input
    {
        cout << "Could not open or find the image" << std::endl ;
        return -1;
    }

	Mat blurred_image;
	blurred_image = 


    namedWindow( "Display window", WINDOW_AUTOSIZE ); // Create a window for display.
    imshow( "Display window", image ); // Show our image inside it.

    waitKey(0); // Wait for a keystroke in the window
    return 0;
}
*/
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

#include "GaussianDegrader.h"



using namespace std;
using namespace cv;


/**
 * function main
 */
 int main( int argc, char** argv )
 {
	 int const MAX_KERNEL_LENGTH = 31;
	char buffer [MAX_KERNEL_LENGTH];
	Mat src; Mat dst;

	string output_location;

	 if( argc != 2)
    {
     cout <<" Usage: blur_wiener ImageToLoadAndDisplay" << endl;
     return -1;
    }

   /// Load the source image
   src = imread(argv[1], IMREAD_COLOR); // Read the file

   
   dst = src.clone();
   
    /// Applying Gaussian blur
    for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 )
    {
		GaussianDegrader degrader(i);
		degrader.process(src, dst);
		/*
		//Genrating Gaussian kernel by hand
		Mat kernel(i, i, CV_32F);//getGaussianKernel(i*6+1, i);
		double center = (double) i / 2.0 - 0.5;
		double sigma = i / 4.0;
		for (int rowInd = 0; rowInd < kernel.rows; rowInd++)
		{
			for (int colInd = 0; colInd < kernel.cols; colInd++)
			{
				double sx = rowInd - center;
				double sy = colInd - center;
				kernel.at<float>(rowInd, colInd) = pow(2.7181, (-sx*sx - sy*sy) / 2 / sigma / sigma); 
			}
		}
		Scalar chSum = sum(kernel);
		double kSum = chSum[0];
		std::cout << kSum << std::endl;
		kernel = kernel / kSum;

		
		
		degradeImage(src, dst, kernel);
		*/

		//GaussianBlur( src, dst, Size( i, i ), 0, 0 );

		//Save the resulting image. Need to construct the path.

		//char *intStr = itoa(i, buffer, 10);
		//string i_s = string(intStr);
		// I'm not using GCC, to this is cool.
		string i_s = to_string(i);
		string argvs = string(argv[1]);
		string extention = ".tiff";
		output_location = "./";
		output_location = "./" + i_s + ".tiff";
		//output_location = output_location.append (i_s, argvs);

		imwrite( output_location, dst );
	}

     return 0;
 }

