//
// Created by jake on 4/14/17.
//

#include "ChainCode.h"

ChainCode::ChainCode(cv::Mat img) : shape2D(img){
	this->chainCode = genChainCode();
}

//use boundary to generate chain code
//algorithm described on page 800 of textbook
std::vector<int> ChainCode::genChainCode() {
	for (Point i : boundary) {
		cout << i.x << ", " << i.y << endl;
	}
}
