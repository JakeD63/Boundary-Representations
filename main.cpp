#include <opencv2/opencv.hpp>

#include "shape2d.hpp"
#include "ShapeNumber.hpp"

using namespace std;
using namespace cv;

void compare(Mat img, int scale);

int main(int argc, char** argv)
{
	if(argc != 3) {
		cerr << "Usage: dip3 image gridScale" << endl;
		exit(0);
	}
	// Load the image
	Mat src = imread(argv[1]);
	
	// Check if everything was fine
	if(!src.data)
		return -1;

	compare(src, atoi(argv[2]));
	return 0;
}

void compare(Mat src, int scale) {
	cvtColor(src, src, CV_BGR2GRAY);
	Mat bin_img;
	threshold(src, bin_img, 128, 255, 1);
	Mat orig = shape2D(bin_img).to_mat();

	auto s = ShapeNumber(bin_img, scale);
	Mat out = s.to_mat();

	imshow("Bound_Orig", orig);
	imshow("SN_Bound", out);
	waitKey(0);
}