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
#include "jpeglib.h"


#include "GaussianDegrader.h"
#include "MathTools.h"


using namespace std;
using namespace cv;


void testWiener(char** argv)
{
	int const MAX_KERNEL_LENGTH = 31;
	char buffer[MAX_KERNEL_LENGTH];

	Mat src; Mat dst;

	string output_location;

	/// Load the source image
	int isColor = 0;
	src = imread(argv[1], isColor); // Read the file

	dst = src.clone();

	/// Applying Gaussian blur
	GaussianDegrader degrader(3);
	degrader.process(src, dst);

	imwrite("blur.bmp", dst);

	cv::Mat kernelImg;
	cv::Mat kernelScaled;
	normalize(degrader.kernel, kernelScaled, 0, 255, CV_MINMAX);
	
	kernelScaled.convertTo(kernelImg, CV_8UC1);
	imwrite("kernel.bmp", kernelImg);
	double k = 0.01;
	Mat deblur;
	MathTools::wienerFilter(dst, degrader.kernel, k, deblur);
	std::cout << deblur.type() << " " << CV_8UC1 << std::endl;
	imwrite("deblur.bmp", deblur);
}

/* This some code to test that libjpeg is doing alright and that the Linker is also sane.*/


/* we will be using this uninitialized pointer later to store raw, uncompressd image */
unsigned char *raw_image = NULL;

/* dimensions of the image we want to write */
int width = 1654;  //640
int height = 2126; //480
int bytes_per_pixel = 3; /* or 1 for GRACYSCALE images */
int color_space = JCS_RGB; /* or JCS_GRAYSCALE for grayscale images */

/**
* read_jpeg_file Reads from a jpeg file on disk specified by filename and saves into the
* raw_image buffer in an uncompressed format.
*
* \returns positive integer if successful, -1 otherwise
* \param *filename char string specifying the file name to read from
*
*/



int read_jpeg_file(char *filename)
{
	/* these are standard libjpeg structures for reading(decompression) */
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	/* libjpeg data structure for storing one row, that is, scanline of an image */
	JSAMPROW row_pointer[1];

	FILE *infile = fopen(filename, "rb");
	unsigned long location = 0;
	int i = 0;

	if (!infile)
	{
		printf("Error opening jpeg file %s\n!", filename);
		return -1;
	}
	/* here we set up the standard libjpeg error handler */
	cinfo.err = jpeg_std_error(&jerr);
	/* setup decompression process and source, then read JPEG header */
	jpeg_create_decompress(&cinfo);
	/* this makes the library read from infile */
	jpeg_stdio_src(&cinfo, infile);
	/* reading the image header which contains image information */
	jpeg_read_header(&cinfo, TRUE);
	/* Uncomment the following to output image information, if needed. */
	/*--
	printf( "JPEG File Information: \n" );
	printf( "Image width and height: %d pixels and %d pixels.\n", cinfo.image_width, cinfo.image_height );
	printf( "Color components per pixel: %d.\n", cinfo.num_components );
	printf( "Color space: %d.\n", cinfo.jpeg_color_space );
	--*/
	/* Start decompression jpeg here */
	jpeg_start_decompress(&cinfo);

	/* allocate memory to hold the uncompressed image */
	raw_image = (unsigned char*)malloc(cinfo.output_width*cinfo.output_height*cinfo.num_components);
	/* now actually read the jpeg into the raw buffer */
	row_pointer[0] = (unsigned char *)malloc(cinfo.output_width*cinfo.num_components);
	/* read one scan line at a time */
	while (cinfo.output_scanline < cinfo.image_height)
	{
		jpeg_read_scanlines(&cinfo, row_pointer, 1);
		for (i = 0; i<cinfo.image_width*cinfo.num_components; i++)
			raw_image[location++] = row_pointer[0][i];
	}
	/* wrap up decompression, destroy objects, free pointers and close open files */
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	free(row_pointer[0]);
	fclose(infile);
	/* yup, we succeeded! */
	return 1;
}

/**
* write_jpeg_file Writes the raw image data stored in the raw_image buffer
* to a jpeg image with default compression and smoothing options in the file
* specified by *filename.
*
* \returns positive integer if successful, -1 otherwise
* \param *filename char string specifying the file name to save to
*
*/
int write_jpeg_file(char *filename)
{
	struct jpeg_compress_struct cinfo;
	struct jpeg_error_mgr jerr;

	/* this is a pointer to one row of image data */
	JSAMPROW row_pointer[1];
	FILE *outfile = fopen(filename, "wb");

	if (!outfile)
	{
		printf("Error opening output jpeg file %s\n!", filename);
		return -1;
	}
	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, outfile);

	/* Setting the parameters of the output file here */
	cinfo.image_width = width;
	cinfo.image_height = height;
	cinfo.input_components = bytes_per_pixel;
	//cinfo.in_color_space = color_space;
	/* default compression parameters, we shouldn't be worried about these */

	jpeg_set_defaults(&cinfo);
	cinfo.num_components = 3;
	//cinfo.data_precision = 4;
	cinfo.dct_method = JDCT_FLOAT;
	jpeg_set_quality(&cinfo, 15, TRUE);
	/* Now do the compression .. */
	jpeg_start_compress(&cinfo, TRUE);
	/* like reading a file, this time write one row at a time */
	while (cinfo.next_scanline < cinfo.image_height)
	{
		row_pointer[0] = &raw_image[cinfo.next_scanline * cinfo.image_width * cinfo.input_components];
		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}
	/* similar to read file, clean up after we're done compressing */
	jpeg_finish_compress(&cinfo);
	jpeg_destroy_compress(&cinfo);
	fclose(outfile);
	/* success code is 1! */
	return 1;
}



/**
 * function main
 */
 int main( int argc, char** argv )
 {

	testWiener(argv);
//	return 0; Do this and nothing after will happen 

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
	/* This some code to test that libjpeg is doing alright.*/
	//j_compress_ptr cinfo; //This one is OK.
	//jpeg_abort_compress(cinfo); //This one is not if Linker is crazy.

	/*Now, actually doing something.*/
	char *infilename = "test.jpg", *outfilename = "test_out.jpg";

	/* Try opening a jpeg*/
	if (read_jpeg_file(infilename) > 0)
	{
		/* then copy it to another file */
		if (write_jpeg_file(outfilename) < 0) return -1;
	}
	else return -1;


     return 0;
 }

