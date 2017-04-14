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
	auto smat = shape.to_mat();
	imshow("Shape: ", smat);
	auto fd = FourierDescriptor(shape);
	auto nshape = fd.to_shape2D();
	
	imshow("FD: ", nshape.to_mat());
	
	fd.set_desc_count(528);
	
	auto fshape = fd.to_shape2D();
	imshow("FD(40) :", fshape.to_mat());

	waitKey(0);

	return 0;
}
