#include "shape2d.hpp"
#include "ShapeNumber.hpp"
#include <opencv2/opencv.hpp>


struct shapenumber_gui {
	int wHeight, wWidth, gridScale = 1, maxScale;
	string wName;
	ShapeNumber sn;

	shapenumber_gui(Mat img, int height, int width, string name) :
			sn(img),
			wName(name),
			wHeight(height),
			wWidth(width) {
		maxScale = sn.getMaxGridScale();
		namedWindow(wName);
		resizeWindow(wName, 600, 600);
		imshow(wName, sn.to_mat());
	}

};

void showSnGui(int trac, void* data) {
	shapenumber_gui* temp = (shapenumber_gui*) data;
	temp->sn.rescaleBoundary(trac);
	imshow(temp->wName, temp->sn.to_mat());
}

/**
 * This struct is a way to get out of using global variables to controll the
 * fourier descriptor window
 */
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

/**
 * This is the callback function called by the trackbar to update the fourier
 * descriptor window
 */
void showFdGui(int trac, void* data)
{
	fd_gui* temp = (fd_gui*) data;
	temp->fd.reconstruct(trac);
	imshow(temp->wName, temp->fd.to_mat());
}

