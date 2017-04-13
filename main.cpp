#include <opencv2/opencv.hpp>
#include <iostream>
#include "fourier_desc.hpp"
#include "shape2d.hpp"

using namespace std;
using namespace cv;


int main(int, char** argv)
{
	// Load the image
	Mat src = imread(argv[1]);
	
	// Check if everything was fine
	if (!src.data)
		return -1;

	cvtColor(src, src, CV_BGR2GRAY);
	Mat bin_img;
	threshold(src, bin_img, 128, 255, 1);

	auto shape = shape2D(bin_img);	
	auto fd = FourierDescriptor(shape);
	auto m = fd.to_mat();

	imshow("Fourier", m.to_mat());

	return 0;
}
