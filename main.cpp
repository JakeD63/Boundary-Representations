#include <opencv2/opencv.hpp>
#include <iostream>

#include "shape2d.hpp"
#include "fourier_desc.hpp"
#include "ShapeNumber.hpp"
#include <string>

using namespace std;
using namespace cv;

void createDescTrackbar(Size wSize, int max_desc);
void createShapeTrackbar(Size wSize, int max_grid);
void showDesc(int, void*);
void showShape(int, void*);

//Globals
Mat bin_img;
int desc_track_pos, shape_track_pos;
string descriptorName = "desc";
string shapeNumberName = "shape";

//quick and dirty gui w/trackbar
//showShape sometimes crashes (depends on computer?), will debug
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

	shape2D sample = shape2D(bin_img);
	Size wSize = sample.to_mat().size();

	//createDescTrackbar(wSize, sample.getBoundSize());
	createShapeTrackbar(wSize, wSize.width);
	waitKey(0);

	return 0;
}

void createDescTrackbar(Size wSize, int max_desc) {

	desc_track_pos = max_desc / 2;
	namedWindow(descriptorName);
	resizeWindow(descriptorName, wSize.width, wSize.height);
	createTrackbar("Descriptor Count", descriptorName, &desc_track_pos, max_desc, showDesc);
	showDesc(desc_track_pos, 0);
}

void createShapeTrackbar(Size wSize, int max_grid) {
	shape_track_pos = 1;
	namedWindow(shapeNumberName);
	resizeWindow(shapeNumberName, 2*wSize.width, wSize.height);
	createTrackbar("Shape Grid Scale", shapeNumberName, &shape_track_pos, max_grid, showShape);
	showShape(desc_track_pos, 0);
}

void showDesc(int, void*) {
	auto fd = FourierDescriptor(bin_img);
	fd.reconstruct(desc_track_pos);
	auto m = fd.to_mat();
	imshow(descriptorName, m);
	m.release();
}

void showShape(int, void*) {
	Mat shapeLR;
	auto shapeL = ShapeNumber(bin_img, shape_track_pos).to_mat();
	auto shapeR = ShapeNumber(bin_img, shape_track_pos).to_connected_mat();
	hconcat(shapeL, shapeR, shapeLR);
	cout << "grid size" << shape_track_pos << endl;
	imshow(shapeNumberName, shapeLR);
	shapeL.release();
	shapeR.release();
	shapeLR.release();
}