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
	}

};

void showSnGui(int trac, void* data) {
	cout << "callback started" << endl;
	shapenumber_gui* temp = (shapenumber_gui*) data;
	cout << "dereferenced date" << endl;
	temp->sn.rescaleBoundary(trac);
	cout << "recale call done" << endl;
	imshow(temp->wName, temp->sn.to_mat());
	cout << "callback finished" << endl;
}