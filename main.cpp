#include <opencv2/opencv.hpp>
#include <iostream>

#include "shape2d.hpp"
#include "fourier_desc.hpp"
#include "ShapeNumber.hpp"
#include <ostream>
#include <iterator>
#include <string>

using namespace std;
using namespace cv;

void createDescTrackbar(Size wSize, int max_desc);
void createShapeTrackbar(Size wSize, ShapeNumber s, int max_grid);
void showDesc(int, void* data);
void showShape(int, void* data);

//Globals
Mat bin_img;
string descriptorName = "desc";
string shapeNumberName = "shape";
int desc_track_pos, shape_track_pos;
int max_desc, max_grid;


//quick and dirty gui w/trackbar
//crashes sometimes (probably memory allocation related)
//have fixed it partially by resuing shapenumber objects
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

	FourierDescriptor fd = FourierDescriptor(bin_img);

	ShapeNumber s = ShapeNumber(bin_img);
	Size wSize = fd.to_mat().size();

	max_desc = fd.getBoundSize();
	max_grid = s.getMaxGridScale();

	desc_track_pos = 50;
	shape_track_pos = 1;
	//set up descriptor window
	namedWindow(descriptorName);
	resizeWindow(descriptorName, wSize.width, wSize.height);
	createTrackbar("Descriptor Count", descriptorName, &desc_track_pos, max_desc, showDesc, &fd);

	//set up shape number window
	namedWindow(shapeNumberName);
	resizeWindow(shapeNumberName, wSize.width, wSize.height);
	createTrackbar("Shape Grid Scale", shapeNumberName, &shape_track_pos, max_grid, showShape, &s);

	//display images before trackbar has moved
	s.rescaleBoundary(1);
	fd.reconstruct(1);
	imshow(shapeNumberName, s.to_mat());
	imshow(descriptorName, fd.to_mat());
	waitKey(0);

	return 0;
}

void showDesc(int, void* data) {
	FourierDescriptor fd(bin_img);
	fd.reconstruct(desc_track_pos);
	//fd.reconstruct(((desc_track_pos / 100.0) * max_desc));
	imshow(descriptorName, fd.to_mat());
}

void showShape(int, void* data) {
	static Mat shape;
	ShapeNumber *s = (ShapeNumber *)data;
	s->rescaleBoundary(shape_track_pos);
	shape = s->to_mat();
	imshow(shapeNumberName, shape);
}