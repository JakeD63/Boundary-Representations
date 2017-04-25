#include "ChainCode.hpp"
/**
 * Constructor for Chain Code Object
 */
ChainCode::ChainCode(Mat img) : shape2D(img) {
	genChainCode();
	normalizeRot();
}

/**
 * \brief Generates chain code for boundary
 *
 * Uses directions 0-7 for directions, with 0 being ->
 */
void ChainCode::genChainCode() {
	Point c, n;
	int xDiff, yDiff, code;
	//walk boundary getting directions to next point on boundary
	for (int i = 0; i < boundary.size() - 1; i++) {
		c = boundary.at(i);
		n = boundary.at(i + 1);
		xDiff = n.x - c.x;
		yDiff = n.y - c.y;
		//store direction to next point in chain code
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

/**
 * \brief Normalize code for rotation
 *
 * Store difference between rotations in code.
 * This makes the code rotation invariant, so it can
 * more easily be used and compared to other codes.
 */
void ChainCode::normalizeRot() {
	vector<int> tempCode;
	int r;
	//get difference for each point to next point on boundary
	for (int i = 0; i < this->chainCode.size() - 1; i++) {
		r = this->chainCode.at(i + 1) - this->chainCode.at(i);
		if (r < 0)
			r += 8;
		tempCode.push_back(r);

	}
	this->chainCode = tempCode;

}

/**
 * \brief Turns scaled boundary into mat image for display
 *
 * redraws points onto img and returns it
 */
Mat ChainCode::to_mat() {
	this->img = Mat::zeros(max_y + 2, max_x + 2, CV_8UC1);
	for(unsigned int i = 0; i < boundary.size(); i++) {
		this->img.at<uchar>(boundary[i]) = 255;
	}

	return this->img;
}

/**
 * \brief Turns scaled boundary into mat image for display.
 * The image has its points connected
 *
 * redraws points onto img, then draws lines between all points
 */
Mat ChainCode::to_connected_mat() {
	//use cvLine to draw lines between all points
	this->connectedImg = this->to_mat();
	for(int i = 0; i < boundary.size() - 1; i++) {
		line(this->connectedImg, boundary.at(i), boundary.at(i+1), Scalar(255,255,255),1,CV_AA );
	}
	return this->connectedImg;
}

/**
 * \brief Returns the shape chain code
 *
 * return chain code vector
 */
vector<int> ChainCode::getCode() {
	return this->chainCode;
}

/**
 * \brief Get specific number from code
 *
 * at function for chain code
 */
int ChainCode::at(unsigned int i) {
	return this->chainCode.at(i);
}

/**
 * \brief Get the size of the chain code
 *
 * Return the size of the chain code vector
 */
int ChainCode::size() {
	return (int) this->chainCode.size();
}

/**
 * \brief Overloaded print operator for chain code
 *
 * Allows the use of << for printing chain codes
 */
ostream &operator<<(ostream &os, const ChainCode &s) {
	for (auto i : s.chainCode)
		os << i;
	return os;
}

/**
 * \brief Overloaded [] operator for chain code vector
 *
 * [] operator for chain code, mimics vector
 */
int& ChainCode::operator[](unsigned int i) {
	return this->chainCode.at(i);
}