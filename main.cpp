#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

#include "shape2d.hpp"
#include "fourier_desc.hpp"
#include "ShapeNumber.hpp"

#include "gui.cpp"

using namespace std;
using namespace cv;

const string HELP = "usage: ./dip3 <imagefile>";

/**
 * Simply convert color and threshold the image
 */
void preProcess(Mat bimg, Mat& res)
{
	cvtColor(bimg, bimg, CV_BGR2GRAY);
	threshold(bimg, res, 128, 255, 1);
}

int main(int argc, char **argv)
{
	string fdWin = string("Fourier Descriptors");
	if ( argc < 2 )
	{
		cout << HELP << endl;
		return 1;
	}

	Mat res_img;
	Mat img = imread(argv[1]);
	if( !img.data )
		return 2;

	preProcess(img, res_img); 

	// TODO: Insert ShapeD code here

	namedWindow(fdWin, WINDOW_NORMAL);
	resizeWindow(fdWin, 600, 600);
		
	fd_gui fg(res_img, 600, 600, fdWin);
	createTrackbar("Descriptor Count", fdWin, &fg.desc_count, fg.max_d, showFdGui, &fg);

	waitKey(0);
	return 0;


}

