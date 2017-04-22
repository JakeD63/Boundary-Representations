#include <opencv2/opencv.hpp>
#include <iostream>

#include "shape2d.hpp"
#include "fourier_desc.hpp"


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
	auto res = shape.to_mat();

	auto fd = FourierDescriptor(bin_img);
	fd.reconstruct(16);

	fd.reconstruct(38);

	auto m = fd.to_mat();
	imshow("Result:", m);
	waitKey(0);


	return 0;
}
