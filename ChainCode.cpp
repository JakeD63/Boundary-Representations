//
// Created by jake on 4/14/17.
//

#include "ChainCode.hpp"

ChainCode::ChainCode(cv::Mat img, int gridScale) : shape2D(img), gridScale(gridScale) {
	scaleBoundary();
	genChainCode();
	getMinMagnitude();
	normalizeRot();
}

//make grid larger, so we only take
//a sampling of points along the boundary
//this allows similar shapes to have much
//similar chain codes

//take distance to each corner from point
//closest corner is added to new boundary point list
void ChainCode::scaleBoundary() {
	Point c, tl, tr, bl, br;
	vector<int> distances; //tl, tr, bl, br
	vector<Point> boundary;
	int min_p;
	for (int i = 0; i < this->boundary.size(); i++) {
		c = this->boundary.at(i);
		//upper left
		tl.x = roundDown(c.x, gridScale);
		tl.y = roundUp(c.y, gridScale);
		//upper right
		tr.x = roundUp(c.x, gridScale);
		tr.y = roundUp(c.y, gridScale);
		//bottom left
		bl.x = roundDown(c.x, gridScale);
		bl.y = roundDown(c.y, gridScale);
		//bottom right
		br.x = roundUp(c.x, gridScale);
		br.y = roundDown(c.y, gridScale);

		//get min distance
		distances.push_back(distance(c, tl));
		distances.push_back(distance(c, tr));
		distances.push_back(distance(c, bl));
		distances.push_back(distance(c, br));

		min_p = (int) std::distance(distances.begin(), min_element(distances.begin(), distances.end()));
		switch (min_p) {
			case 0:
				boundary.push_back(tl);
				break;
			case 1:
				boundary.push_back(tr);
				break;
			case 2:
				boundary.push_back(bl);
				break;
			case 3:
				boundary.push_back(br);
				break;
			default:
				cerr << "scaleBoundary: min_p not in range 0-3: " << min_p << endl;
				exit(0);
		}
		//clear distances vector
		distances.clear();
	}
	//add unique points to scaled boundary
	for (int i = 0; i < boundary.size() - 1; i++) {
		if (!(boundary.at(i) == boundary.at(i + 1)))
			scaldedBoundary.push_back(boundary[i]);
	}

}

//use boundary to generate chain code
//directions and method from page 800
void ChainCode::genChainCode() {
	Point c, n;
	int xDiff, yDiff, code;
	for (int i = 0; i < scaldedBoundary.size() - 1; i++) {
		c = scaldedBoundary.at(i);
		n = scaldedBoundary.at(i + 1);
		xDiff = n.x - c.x;
		yDiff = n.y - c.y;
		if (xDiff == gridScale) {
			if (yDiff == gridScale)
				code = 1;
			else if (yDiff == 0)
				code = 0;
			else if (yDiff == -gridScale)
				code = 7;
		} else if (xDiff == 0) {
			if (yDiff == gridScale)
				code = 2;
			else if (yDiff == -gridScale)
				code = 6;
		} else if (xDiff == -gridScale) {
			if (yDiff == gridScale)
				code = 3;
			else if (yDiff == 0)
				code = 4;
			else if (yDiff == -gridScale)
				code = 5;
		}
		chainCode.push_back(code);
	}
}

//treat vector as integer
//get min of all rotations
//NOTE: this uses string to convert
//a vector to an int for comparison
//it is super gross
void ChainCode::getMinMagnitude() {
	vector<int> min = chainCode; //set starting min
	vector<int> curRot = chainCode;
	for(int i = 0; i < chainCode.size(); i++) {

		if(compareCodes(curRot, min) == -1)
			min = curRot;

		//rotate again
		rotate(curRot.begin(), curRot.end()-1, curRot.end());
	}
	this->chainCode = min;
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
	for ( unsigned int i = scaldedBoundary.size(); i-- > 0; )
	{
		output.at<uchar>(scaldedBoundary[i]) = 255;
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

//convert chain code vector into integer
//for magnitude comparisons
int ChainCode::compareCodes(vector<int> a, vector<int> b) {
	if(a.size() != b.size()) {
        cerr << "compareCodes: vector sizes must be equal" << endl;
        exit(0);
    }
  //walk a and b until numbers are different, then return a<b
  for(int i = 0; i < a.size(); i++) {
    if(a[i] < b[i])
      return -1;
    else if(a[i] > b[i])
      return 1;
  }
  return 0;
}

int ChainCode::distance(Point a, Point b) {
	return sqrt(pow((b.x - a.x), 2) + pow((b.y - a.y), 2));
}

//rounds n up to nearest multiple of m
int ChainCode::roundUp(int n, int m) {
	int r;
	if (m == 0)
		return n;

	r = n % m;
	if (r == 0)
		return n;
	return n + m - r;
}

//rounds n down to nearest multiple of m
int ChainCode::roundDown(int n, int m) {
	return roundUp(n - m, m);
}

ostream &operator<<(ostream &os, const ChainCode &cc) {
	for (auto i : cc.chainCode)
		os << i;
	return os;
}

int& ChainCode::operator[](unsigned int i) {
	return this->chainCode.at(i);
}


