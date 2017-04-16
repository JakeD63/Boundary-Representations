#include <opencv2/opencv.hpp>

#include <iostream>

#include "shape2d.hpp"
#include "fourier_desc.hpp"

using namespace std;
using namespace cv;

void compare(Mat img1, Mat img2, int scale);

int main(int argc, char** argv)
{
	if(argc != 4) {
		cerr << "Usage: dip3 image1 image2 gridScale" << endl;
		exit(0);
	}
	// Load the image
	Mat img1 = imread(argv[1]);
	Mat img2 = imread(argv[2]);
	
	// Check if everything was fine
	if(!img1.data || !img2.data)
		return -1;

	compare(img1, img2, atoi(argv[3]));

	return 0;
}

	auto fd = FourierDescriptor(bin_img);
	fd.reconstruct(256);
	auto m = fd.to_mat();

	imshow("Result:", m);
	waitKey(0);


	return 0;
}
