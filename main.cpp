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

void erodeToSingle(Mat bimg, Mat& res, int thresh)
{
	Mat element = getStructuringElement( MORPH_RECT,
			Size( 7, 7 ),
			Point( 3, 3) );

	bool done = false;
	do {
		erode(bimg, res, element);

		FourierDescriptor fd(res);
		if( fd.size() > thresh ) 
			return;

	} while ( !done );


}


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
		max_d = fd.size();
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
	Mat res_img;
	Mat img = imread(argv[1]);

	if( !img.data )
		return 2;

	cvtColor(img, img, CV_BGR2GRAY);
	namedWindow(testW, WINDOW_NORMAL);
	imshow(testW, img);
	resizeWindow(testW, 600, 600);
	
	threshold(img, bin_img, 128, 255, 1);
	imshow(testW, bin_img);
	

	/* erodeToSingle(bin_img, res_img, 500); */
	
	fd_gui fg(bin_img, 600, 600, testW);
	createTrackbar("Descriptor Count", testW, &fg.desc_count, fg.max_d, showFdGui, &fg);

	waitKey(0);
	return 0;


}

