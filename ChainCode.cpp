//
// Created by jake on 4/14/17.
//

#include "ChainCode.h"

ChainCode::ChainCode(cv::Mat img) : shape2D(img){
	genChainCode();
	normalizeRot();
}

//use boundary to generate chain code
//directions and method from page 800
void ChainCode::genChainCode() {
	Point c, n;
	int xDiff, yDiff, code;
	for (int i = 0; i < boundary.size() - 1; i++) {
		c = boundary.at(i);
		n = boundary.at(i + 1);
		xDiff = n.x - c.x;
		yDiff = n.y - c.y;
		if(xDiff == 1) {
			if(yDiff == 1)
				code = 1;
			else if(yDiff == 0)
				code = 0;
			else if(yDiff == -1)
				code = 7;
		} else if(xDiff == 0) {
			if(yDiff == 1)
				code = 2;
			else if(yDiff == -1)
				code = 6;
		} else if(xDiff == -1) {
			if(yDiff == 1)
				code = 3;
			else if(yDiff == 0)
				code = 4;
			else if(yDiff == -1)
				code = 5;
		}
		chainCode.push_back(code);
	}
}

//take first difference of directions
//in counterclockwise direction
// c[i+1] - c[i] (+8 if neg)
void ChainCode::normalizeRot() {
	vector<int> tempCode;
	int r;
	for(int i = 0; i < this->chainCode.size() - 1; i++) {
		r = this->chainCode.at(i + 1) - this->chainCode.at(i);
		if(r < 0)
			r += 8;
		tempCode.push_back(r);

	}
	this->chainCode = tempCode;

}


vector<int> ChainCode::getCode() {
	return this->chainCode;
}

ostream &operator<<(ostream &os, const ChainCode &cc) {
	for(auto i : cc.chainCode)
		os << i;
	return os;
}


