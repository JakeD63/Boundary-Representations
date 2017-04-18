//
// Created by jake on 4/14/17.
//

#include "ShapeNumber.hpp"

ShapeNumber::ShapeNumber(cv::Mat img, int scale) : shape2D(img) {
	setGridScale(scale);
	scaleBoundary();
	genChainCode();
	getMinMagnitude();
	normalizeRot();
}

void ShapeNumber::setGridScale(int scale) {
	int max = this->getMaxGridScale();
	if(gridScale > max)
		this->gridScale = max;
	else
		this->gridScale = scale;
}

//get the maximum grid resize allowed by boundary
int ShapeNumber::getMaxGridScale() {
	return this->GRID_MAX;
}

//make grid larger, so we only take
//a sampling of points along the boundary
//this allows similar shapes to have much
//similar chain codes

//take distance to each corner from point
//closest corner is added to new boundary point list
void ShapeNumber::scaleBoundary() {
	Point c, tl, tr, bl, br;
	vector<double> distances; //tl, tr, bl, br
	vector<Point> temp_bound;
	int min_p;
	for (unsigned int i = 0; i < this->boundary.size(); i++) {
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
				temp_bound.push_back(tl);
				break;
			case 1:
				temp_bound.push_back(tr);
				break;
			case 2:
				temp_bound.push_back(bl);
				break;
			case 3:
				temp_bound.push_back(br);
				break;
			default:
				cerr << "scaleBoundary: min_p not in range 0-3: " << min_p << endl;
				exit(0);
		}
		//clear distances vector
		distances.clear();
	}
	//add unique points to scaled boundary
	for (unsigned int i = 0; i < temp_bound.size() - 1; i++) {
		if (!(temp_bound.at(i) == temp_bound.at(i + 1)))
			scaledBoundary.push_back(temp_bound[i]);
	}

}

//use boundary to generate chain code
//directions and method from page 800
void ShapeNumber::genChainCode() {
	Point c, n;
	int xDiff, yDiff, code = 0;
	for (unsigned int i = 0; i < scaledBoundary.size() - 1; i++) {
		c = scaledBoundary.at(i);
		n = scaledBoundary.at(i + 1);
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
		shapeNumber.push_back(code);
	}
}

//treat vector as integer
//get min of all rotations
//NOTE: this uses string to convert
//a vector to an int for comparison
//it is super gross
void ShapeNumber::getMinMagnitude() {
	vector<int> min = shapeNumber; //set starting min
	vector<int> curRot = shapeNumber;
	for(int i = 0; i < shapeNumber.size(); i++) {

		if(compareCodes(curRot, min) == -1)
			min = curRot;

		//rotate again
		rotate(curRot.begin(), curRot.end()-1, curRot.end());
	}
	this->shapeNumber = min;
}


//take first difference of directions
//in counterclockwise direction
// c[i+1] - c[i] (+8 if neg)
void ShapeNumber::normalizeRot() {
	vector<int> tempCode;
	int r;
	for (unsigned int i = 0; i < this->shapeNumber.size() - 1; i++) {
		r = this->shapeNumber.at(i + 1) - this->shapeNumber.at(i);
		if (r < 0)
			r += 8;
		tempCode.push_back(r);

	}
	this->shapeNumber = tempCode;

}

void ShapeNumber::rescaleBoundary(int scale) {
	this->scaledBoundary.clear();
	this->shapeNumber.clear();

	setGridScale(scale);
	scaleBoundary();
	genChainCode();
	getMinMagnitude();
	normalizeRot();
}

//! to_mat generates a normalized mat with our boundary in it.
Mat ShapeNumber::to_mat() {
	this->img = Mat::zeros(max_y + 2, max_x + 2, CV_8UC1);
	for(unsigned int i = 0; i < scaledBoundary.size(); i++) {
		this->img.at<uchar>(scaledBoundary[i]) = 255;
	}
	return this->img;
}

//return a Mat image with points connected with
//white line
Mat ShapeNumber::to_connected_mat() {
	//use cvLine to draw lines between all points
	this->connectedImg = this->to_mat();
	for(int i = 0; i < scaledBoundary.size() - 1; i++) {
		line(this->connectedImg, scaledBoundary.at(i), scaledBoundary.at(i+1), Scalar(255,255,255),1,CV_AA );
	}
	return this->connectedImg;
}



//get shape number
vector<int> ShapeNumber::getCode() {
	return this->shapeNumber;
}

//get specific number from shape number
int ShapeNumber::at(unsigned int i) {
	return this->shapeNumber.at(i);
}

//get number of digits in shape number
int ShapeNumber::size() {
	return (int) this->shapeNumber.size();
}

//convert chain code vector into integer
//for magnitude comparisons
int ShapeNumber::compareCodes(vector<int> a, vector<int> b) {
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

//get distance between two 2D points
double ShapeNumber::distance(Point a, Point b) {
	return sqrt(pow((b.x - a.x), 2) + pow((b.y - a.y), 2));
}

//rounds n up to nearest multiple of m
int ShapeNumber::roundUp(int n, int m) {
	int r;
	if (m == 0)
		return n;

	r = n % m;
	if (r == 0)
		return n;
	return n + m - r;
}

//rounds n down to nearest multiple of m
int ShapeNumber::roundDown(int n, int m) {
	return roundUp(n - m, m);
}

//print operator for shape number
ostream &operator<<(ostream &os, const ShapeNumber &s) {
	for (auto i : s.shapeNumber)
		os << i;
	return os;
}

//access operator for shape number
int& ShapeNumber::operator[](unsigned int i) {
	return this->shapeNumber.at(i);
}