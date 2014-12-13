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
//#include <string>



using namespace std;
using namespace cv;
int const MAX_KERNEL_LENGTH = 31;
char buffer [MAX_KERNEL_LENGTH];
Mat src; Mat dst;

string output_location;

/**
 * function main
 */
 int main( int argc, char** argv )
 {
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
        { GaussianBlur( src, dst, Size( i, i ), 0, 0 );

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

