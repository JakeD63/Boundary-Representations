//
// Created by jake on 4/14/17.
//

#ifndef DIP3_CHAINCODE_H
#define DIP3_CHAINCODE_H

#include "shape2d.hpp"

using namespace std;
using namespace cv;

class ShapeNumber : public shape2D {
public:
	ShapeNumber(Mat img, int scale = 5);
	void rescaleBoundary(int scale);
	Mat to_mat();
	Mat to_connected_mat();
	int getMaxGridScale();
	vector<int> getCode();
	int at(unsigned int i);
	int size();
	friend ostream &operator<<(ostream &os, const ShapeNumber &s);
	int& operator[](unsigned int i);

private:
	void setGridScale(int scale);
	void scaleBoundary();

	void genChainCode();

	void getMinMagnitude();

	void normalizeRot();

	int compareCodes(vector<int> a, vector<int> b);

	double distance(Point a, Point b);

	Mat redrawPoints(Mat img);
	int roundUp(int n, int m);

	int roundDown(int n, int m);

	Mat img, connectedImg;
	int gridScale;
	const int GRID_MAX = 100;
	std::vector<int> shapeNumber;
	std::vector<Point> scaledBoundary;


};


#endif //DIP3_CHAINCODE_H
