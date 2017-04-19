#include <opencv2/opencv.hpp>
#include <iostream>

#include "shape2d.hpp"
#include "fourier_desc.hpp"
#include "ShapeNumber.hpp"
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
	cout << max_desc << endl;
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
	showDesc(desc_track_pos, &fd);
	showShape(shape_track_pos, &s);

	waitKey(0);

	return 0;
}

//I tried making the trackbar range between 0 and 100
//and use position as a percentage of max, but it still weighted
//heavily to lower trackbar numbers. I committed the code, feel free to check it out
void showDesc(int, void* data) {
	Mat m;
	FourierDescriptor fd(bin_img);
	fd.reconstruct(log(desc_track_pos));
	//fd.reconstruct(((desc_track_pos / 100.0) * max_desc));
	m = fd.to_mat();
	imshow(descriptorName, m);
}

void showShape(int, void* data) {
	static Mat shapeL, shapeR, shapeLR;
	ShapeNumber *s = (ShapeNumber *)data;
	s->rescaleBoundary(shape_track_pos);
	shapeL = s->to_mat();
	//shapeR = s->to_connected_mat();
	//hconcat(shapeL, shapeR, shapeLR);
	imshow(shapeNumberName, shapeL);
}