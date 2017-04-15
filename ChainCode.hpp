//
// Created by jake on 4/14/17.
//

#ifndef DIP3_CHAINCODE_H
#define DIP3_CHAINCODE_H

#include "shape2d.hpp"

using namespace std;
using namespace cv;

class ChainCode : public shape2D {
public:
	ChainCode(cv::Mat img, int scale = 5);

	Mat to_mat();

	vector<int> getCode();
	int at(int i);
	int size();
	friend ostream &operator<<(ostream &os, const ChainCode &cc);
	int& operator[](unsigned int i);

private:
	void scaleBoundary();

	void genChainCode();

	void getMinMagnitude();

	void normalizeRot();

	int compareCodes(vector<int> a, vector<int> b);

	int distance(Point a, Point b);

	int roundUp(int n, int m);

	int roundDown(int n, int m);

	int gridScale;
	std::vector<int> chainCode;
	std::vector<Point> scaldedBoundary;


};


#endif //DIP3_CHAINCODE_H
