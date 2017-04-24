#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

#include "shape2d.hpp"
#include "fourier_desc.hpp"
#include "ShapeNumber.hpp"

using namespace std;
using namespace cv;

const string HELP = "usage: ./dip3 <imagefile>";

void showFdGui(int trac, void* data);

struct fd_gui {
	int desc_count, max_d, wHeight, wWidth;
	string wName;
	FourierDescriptor fd;
	
	fd_gui(Mat img, int height, int width, string name) : 
		fd(img),
		wHeight(height),
		wWidth(width),
		wName(name)
	{
		max_d = (int) floor(fd.size() / 4);
		desc_count = (int) floor(max_d / 2);

		namedWindow(wName);
		resizeWindow(wName, wWidth, wHeight);
	};

};	

void showFdGui(int trac, void* data)
{
	fd_gui* temp = (fd_gui*) data;
	temp->fd.reconstruct(trac);
	imshow(temp->wName, temp->fd.to_mat());
}

int main(int argc, char **argv)
{
	string testW = string("TEST");
	if ( argc < 2 )
	{
		cout << HELP << endl;
		return 1;
	}

	Mat bin_img;
	Mat img = imread(argv[1]);

	if( !img.data )
		return 2;

	cvtColor(img, img, CV_BGR2GRAY);
	namedWindow(testW, WINDOW_NORMAL);
	imshow(testW, img);
	resizeWindow(testW, 600, 600);

	threshold(img, bin_img, 128, 255, 1);
	imshow(testW, bin_img);

	shape2D s(bin_img);
	imshow(testW, s.to_mat());
	
	fd_gui fg(bin_img, 600, 600, testW);
	createTrackbar("Descriptor Count", testW, &fg.desc_count, fg.max_d, showFdGui, &fg);

	waitKey(0);
	return 0;


}

