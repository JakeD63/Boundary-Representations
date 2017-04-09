#include <opencv2/opencv.hpp>
#include <iostream>

#include "chaincodes.hpp"

using namespace std;
using namespace cv;

int display_img( string name, Mat dsp );

int main(int, char** argv)
{
    // Load the image
    Mat src = imread(argv[1]);
    // Check if everything was fine
    if (!src.data)
        return -1;

    cvtColor(src, src, CV_BGR2GRAY);
    Mat bin_img;
    threshold(src, bin_img, 128, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

    auto tst  = ChainCode{ bin_img };
    return 0;

}
