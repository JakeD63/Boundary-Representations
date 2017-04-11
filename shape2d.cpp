#include <opencv2/opencv.hpp>
#include <utility>
#include "shape2d.hpp"


using namespace cv;

//! shape2D constructor
//! @param img Expects Mat containing binary thresholded image
shape2D::shape2D(Mat img)
{
	// we only accept 8 bit single channel images
	CV_Assert( img.depth() == CV_8U );

	// Boundary following algorithm described in 11.1.1 of Gonzales and 
	// Woods

	std::pair<unsigned int, unsigned int> tm_lm; 	// topmost, leftmost
	std::pair<unsigned int, unsigned int> b_n; 	// current pixel
	std::pair<unsigned int, unsigned int> c_n; 	// current bg
	
	bool b_found = false;	// true after we find b_0

	for( int r = 0; r < img.rows; r++ )
	{
		for( int c = 0; c < img.cols; c++ )
		{
			Scalar intensity = img.at<uchar>(r, c);

			// case 1: have not encountered border yet
			if( !b_found && intensity.val[0] == 0 )
				continue;
	
			// case 2: found topmost, leftmost pixel	
			if( !b_found && intensity.val[0] == 255 )	
			{
				b_found = true;
				tm_lm = std::make_pair(r, c);
				b_n = std::make_pair(r, c);
				c_n = std::make_pair(r, c-1);
			}
			
			for( int cp = 0; cp < 8; cp++ )
			{
				
			}
							
		}
	}
}
