#include <opencv2/opencv.hpp>
<<<<<<< HEAD
#include "ShapeNumber.hpp"
=======
#include <iostream>

#include "shape2d.hpp"
#include "fourier_desc.hpp"

>>>>>>> b521b1ff6c9259bbe50375aeafa5c58486071115

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

<<<<<<< HEAD
void compare(Mat img1, Mat img2, int scale) {
	Mat bin_img1, bin_img2;

	cvtColor(img1, img1, CV_BGR2GRAY);
	cvtColor(img2, img2, CV_BGR2GRAY);
	threshold(img1, bin_img1, 128, 255, 1);
	threshold(img2, bin_img2, 128, 255, 1);

	Mat out1 = ShapeNumber(bin_img1, scale).to_connected_mat();
	Mat out2 = ShapeNumber(bin_img2, scale).to_connected_mat();

	imshow("Image1", out1);
	imshow("Image2", out2);

	waitKey(0);
=======
	auto fd = FourierDescriptor(bin_img);
	fd.reconstruct(256);
	auto m = fd.to_mat();

	imshow("Result:", m);
	waitKey(0);


	return 0;
>>>>>>> b521b1ff6c9259bbe50375aeafa5c58486071115
}
