//
// Created by jake on 4/15/17.
//

#include "ChainCode.hpp"

ChainCode::ChainCode(Mat img) : shape2D(img) {
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
		if (xDiff == 1) {
			if (yDiff == 1)
				code = 1;
			else if (yDiff == 0)
				code = 0;
			else if (yDiff == -1)
				code = 7;
		} else if (xDiff == 0) {
			if (yDiff == 1)
				code = 2;
			else if (yDiff == -1)
				code = 6;
		} else if (xDiff == -1) {
			if (yDiff == 1)
				code = 3;
			else if (yDiff == 0)
				code = 4;
			else if (yDiff == -1)
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
	for (int i = 0; i < this->chainCode.size() - 1; i++) {
		r = this->chainCode.at(i + 1) - this->chainCode.at(i);
		if (r < 0)
			r += 8;
		tempCode.push_back(r);

	}
	this->chainCode = tempCode;

}

//! to_mat generates a normalized mat with our boundary in it.
Mat ChainCode::to_mat() {
	Mat output = Mat::zeros(max_y + 2, max_x + 2, CV_8UC1);
	for ( unsigned int i = boundary.size(); i-- > 0; )
	{
		output.at<uchar>(boundary[i]) = 255;
	}

	return output;
}

vector<int> ChainCode::getCode() {
	return this->chainCode;
}

int ChainCode::at(int i) {
	return this->chainCode.at(i);
}

int ChainCode::size() {
	return this->chainCode.size();
}


ostream &operator<<(ostream &os, const ChainCode &s) {
	for (auto i : s.chainCode)
		os << i;
	return os;
}

int& ChainCode::operator[](unsigned int i) {
	return this->chainCode.at(i);
}