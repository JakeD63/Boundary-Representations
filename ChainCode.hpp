//
// Created by jake on 4/15/17.
//

#ifndef DIP3_CHAINCODE_HPP
#define DIP3_CHAINCODE_HPP

#include "shape2d.hpp"
using namespace std;
using namespace cv;

class ChainCode : public shape2D {
public:
	ChainCode(Mat img);

	Mat to_mat();

	vector<int> getCode();
	int at(int i);
	int size();
	friend ostream &operator<<(ostream &os, const ChainCode &cc);
	int& operator[](unsigned int i);

private:

	void genChainCode();

	void normalizeRot();

	std::vector<int> chainCode;

};


#endif //DIP3_CHAINCODE_HPP