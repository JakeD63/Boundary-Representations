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
FourierDescriptor* fdp;
ShapeNumber* snp;

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
	fdp = &fd;
	ShapeNumber s = ShapeNumber(bin_img);
	snp = &s;
	Size wSize = fd.to_mat().size();

	max_desc = (int) floor(fd.getBoundSize() / 5);
	max_grid = s.getMaxGridScale();

	desc_track_pos = 50;
	shape_track_pos = 1;

	//set up descriptor window
	namedWindow(descriptorName);
	resizeWindow(descriptorName, wSize.width, wSize.height);
	createTrackbar("Descriptor Count", descriptorName, &desc_track_pos, max_desc, showDesc);

	//set up shape number window
	namedWindow(shapeNumberName);
	resizeWindow(shapeNumberName, wSize.width, wSize.height);
	createTrackbar("Shape Grid Scale", shapeNumberName, &shape_track_pos, max_grid, showShape);

	//display images before trackbar has moved
	fdp->reconstruct(max_desc);
	snp->rescaleBoundary(1);
	imshow(descriptorName, fdp->to_mat());
	imshow(shapeNumberName, snp->to_mat());

	waitKey(0);

	return 0;
}

void showDesc(int, void* data) {
	Mat m;
	// *fdp.reconstruct(log(desc_track_pos));
	fdp->reconstruct(desc_track_pos);
	m = fdp->to_mat();
	imshow(descriptorName, m);
}

void showShape(int, void* data) {
	snp->rescaleBoundary(shape_track_pos);

	imshow(shapeNumberName, snp->to_mat());
}
