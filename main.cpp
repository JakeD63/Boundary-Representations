#include <opencv2/opencv.hpp>
#include <iostream>

#include "shape2d.hpp"
#include "fourier_desc.hpp"
#include "ShapeNumber.hpp"
#include <string>

using namespace std;
using namespace cv;

void createDescTrackbar();
void createShapeTrackbar();
void showDesc(int, void*);
void showShape(int, void*);

//Globals
Mat bin_img;
int desc_track_pos, desc_track_max;
int shape_track_pos, shape_track_max;
string descriptorName = "desc";
string shapeNumberName = "shape";

//quick and dirty gui w/trackbar
//showShape sometimes crashes, will debug
//not related to our classes, crashes before showShape func call
//looks like its happening in Mat deallocation
int main(int, char** argv)
{
	// Load the image
	Mat img = imread(argv[1]);
	
	// Check if everything was fine
	if (!img.data)
		return -1;

	cvtColor(img, img, CV_BGR2GRAY);

	threshold(img, bin_img, 128, 255, 1);

	createDescTrackbar();
	createShapeTrackbar();
	waitKey(0);

	return 0;
}

void createDescTrackbar() {
	shape2D sample = shape2D(bin_img);
	Size wSize = sample.to_mat().size();
	desc_track_max = sample.getBoundSize();
	desc_track_pos = desc_track_max / 2;
	namedWindow(descriptorName);
	resizeWindow(descriptorName, wSize.width, wSize.height);
	createTrackbar("Descriptor Count", descriptorName, &desc_track_pos, desc_track_max, showDesc);
	showDesc(desc_track_pos, 0);
}

void createShapeTrackbar() {
	shape2D sample = shape2D(bin_img);
	Size wSize = sample.to_mat().size();
	shape_track_max = 80;
	shape_track_pos = 5;
	namedWindow(shapeNumberName);
	resizeWindow(shapeNumberName, 2*wSize.width, wSize.height);
	createTrackbar("Shape Grid Scale", shapeNumberName, &shape_track_pos, shape_track_max, showShape);
	showShape(desc_track_pos, 0);
}

void showDesc(int, void*) {
	auto fd = FourierDescriptor(bin_img);
	fd.reconstruct(desc_track_pos);
	auto m = fd.to_mat();
	imshow(descriptorName, m);
}

void showShape(int, void*) {
	cout << "showshape call" << endl; //crashes before here
	Mat shapeLR;
	auto shapeL = ShapeNumber(bin_img, shape_track_pos).to_mat();
	auto shapeR = ShapeNumber(bin_img, shape_track_pos).to_connected_mat();
	hconcat(shapeL, shapeR, shapeLR);
	imshow(shapeNumberName, shapeLR);

	cout << "image displayed" << endl;
}