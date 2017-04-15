#include <opencv2/opencv.hpp>
#include <iostream>

#include "shape2d.hpp"
#include "ChainCode.hpp"

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

    auto cc = ChainCode(bin_img);
	cout << cc;

	Mat out = cc.to_mat();

	imshow("BINIMG", out);
	waitKey(0);

	return 0;
}
