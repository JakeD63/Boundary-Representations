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

	Mat to_mat();

	vector<int> getCode();
	int at(int i);
	int size();
	friend ostream &operator<<(ostream &os, const ShapeNumber &s);
	int& operator[](unsigned int i);

private:
	void scaleBoundary();

	void genChainCode();

	void getMinMagnitude();

	void normalizeRot();

	int compareCodes(vector<int> a, vector<int> b);

	double distance(Point a, Point b);

	int roundUp(int n, int m);

	int roundDown(int n, int m);

	int gridScale;
	std::vector<int> shapeNumber;
	std::vector<Point> scaldedBoundary;


};


#endif //DIP3_CHAINCODE_H
