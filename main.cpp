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
	Size wSize = fd.to_mat().size();

	max_desc = (int) floor(fd.getBoundSize() / 5);
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

void showDesc(int, void* data) {
	Mat m;
	// *fdp.reconstruct(log(desc_track_pos));
	fdp->reconstruct(desc_track_pos);
	m = fdp->to_mat();
	imshow(descriptorName, m);
}

void showShape(int, void* data) {
	static Mat shapeL, shapeR, shapeLR;
	ShapeNumber *s = (ShapeNumber *)data;
	vector<int> code;
	s->rescaleBoundary(shape_track_pos);
	code = s->getCode();
	shapeL = s->to_mat();
	std::stringstream codeString;
	std::copy(code.begin(), code.end(), std::ostream_iterator<int>(codeString, ""));
	putText(shapeL, codeString.str().c_str(), Point(30,30), FONT_HERSHEY_SCRIPT_SIMPLEX, 0.2, Scalar(255,255,255), 1, CV_AA);
	//shapeR = s->to_connected_mat();
	//hconcat(shapeL, shapeR, shapeLR);
	imshow(shapeNumberName, shapeL);
}
