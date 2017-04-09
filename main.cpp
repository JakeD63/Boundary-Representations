#include <opencv2/opencv.hpp>
#include <iostream>
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

    Mat src_gray;
    cvtColor( src, src_gray, CV_BGR2GRAY);

    Mat blr_gray;
    blur( src_gray, blr_gray, Size(3,3));

    Mat edges;
    Canny( blr_gray, edges, 60, 80, 3 );

    Mat dst;
    dst = Scalar::all(0);

    src.copyTo(dst, edges);

    Mat bin;
    cvtColor(dst, dst, CV_BGR2GRAY);
    threshold(dst, bin, 40, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);

    display_img(string("PRE-MORPH"), bin);
/*
    Mat morph;
    Mat element = getStructuringElement(MORPH_CROSS, Size(3, 3), Point(1, 1) );

    morphologyEx(bin, morph, MORPH_OPEN, element);

    display_img(string("BLACKHAT"), morph);

    Mat dist;
    distanceTransform(morph, dist, CV_DIST_L2, 3);
    normalize(dist, dist, 0, 1., NORM_MINMAX);

    display_img(string("Distance"), dist);
*/
    return 0;
}

int display_img( string name, Mat dsp )
{
    imshow(name, dsp);
    int c = waitKey(0);
    return c;
}
