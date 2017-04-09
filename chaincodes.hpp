
#ifndef __ChainCode_HPP
#define __ChainCode_HPP

#include <opencv2/opencv.hpp>
#include <vector>

// Setup directions
const unsigned char D_N  = 0; //!< North
const unsigned char D_NE = 1; //!< North East
const unsigned char D_E  = 2; //!< East
const unsigned char D_SE = 3; //!< South East 
const unsigned char D_S  = 4; //!< South
const unsigned char D_SW = 5; //!< South West
const unsigned char D_W  = 6; //!< West
const unsigned char D_NW = 7; //!< North West

//! ChainCode provides code and data structure for a chain code representation
//! of a shape within an image.
class ChainCode {

	private:
		//! image input
		cv::Mat iimage;

		//! chaincode representation
		std::vector<unsigned char> chain;

		//! start point of chaincode
		unsigned int start;	

	public:
		//! basic constructor
		ChainCode(cv::Mat img);

		//! basic destructor
		~ChainCode();
};

#endif
