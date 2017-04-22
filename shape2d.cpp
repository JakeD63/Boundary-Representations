#include <opencv2/opencv.hpp>
#include <iostream>
#include "shape2d.hpp"


using namespace cv;

//! rotate_cw does a clockwise rotation of a single point mask around point p
//! @param p point to rotate around
//! @param c_n point to rotate
void shape2D::rotate_cw(const Point p, Point& c_n)
{
	// Case: c_n is north or south of p
	if( p.x == c_n.x ) 		// N or S 
	{
		if ( p.y < c_n.y ) 	// N 
		{
			c_n.x++;	// now NE
			return;
		} else {		// S 
			c_n.x--;	// now SE
			return;		
		}
	} 

	// Case: c_n is east or west of p
	if( p.y == c_n.y )		// E or W 
	{
		if( p.x < c_n.x )	// E 
		{
			c_n.y--;	// now SE
			return;
		} else {		// W 
			c_n.y++;	// now NW
			return;
		}
	}

	// Case: c_n is in NW corner 
	if( c_n.x < p.x && c_n.y > p.y) 
	{
		c_n.x++;
		return;
	}

	// Case: c_n is in NE corner
	if( c_n.x > p.x && c_n.y > p.y )
	{
		c_n.y--;
		return;
	}

	// Case: c_n is in SE corner
	if( c_n.x > p.x && c_n.y < p.y )
	{
		c_n.x--;
		return;
	}

	// Case: c_n is in SW corner
	if( c_n.x < p.x &&  c_n.y < p.y)
	{
		c_n.y++;
		return;
	}


}

Point shape2D::find_tm_lm(Mat img)
{	

	for( int y = img.rows - 1; y >= 0; y--)
	{
		for ( int x = 0; x < img.cols; x++ )
		{
			Scalar i = img.at<uchar>(Point(x, y));
			if (i.val[0] == 255)
				return Point(x, y);
		}
	}

	return Point(0, img.cols);
}

//! shape2D constructor
//! @param img Expects Mat containing binary thresholded image
shape2D::shape2D(Mat img)
{
	this->imgSize = img.size();
	// we only accept 8 bit single channel images
	CV_Assert( img.depth() == CV_8U );

	// Boundary following algorithm described in 11.1.1 of Gonzales and 
	// Woods

	// Get the topmost leftmost
	auto tm_lm = find_tm_lm(img);
	//std::cout << "tm_lm value: " << tm_lm.x << " " << tm_lm.y << std::endl;

	boundary.push_back(tm_lm);

	bool done = false;

	// Set c_n to the column west of the topmost leftmost of the shape
	Point c_n = tm_lm;
	c_n.x--;

	// init max and min values 
	max_y = tm_lm.y;
	min_y = tm_lm.y;
	max_x = tm_lm.x;
	min_x = tm_lm.x;

	//std::cout << "c_n initialized " << c_n.x << " " << c_n.y << std::endl;

	//std::cout << "Beginning boundary follow" << std::endl;


	while(!done)	
	{


		Point save; 	// Save last c_n for next loop
		Scalar in;	// Intensity of c_n
		// starting from c_n walk clockwise
		
		do
		{
			save = c_n;
			rotate_cw(boundary.back(), c_n);
			in = img.at<uchar>(c_n);

			//std::cout << "Rotated: " << c_n.x << " " << c_n.y << " " << in.val[0] << std::endl;

		} while(in.val[0] != 255);
		
		// base case
		if (boundary[0] == boundary.back() && c_n == boundary[1])
			done = true;

		// if not the base case then continue
		if (!done)		
		{
			if ( c_n.x > max_x)
				max_x = c_n.x;

			if ( c_n.x < min_x )
				min_x = c_n.x;

			if ( c_n.y < min_y )
				min_y = c_n.y;

			boundary.push_back(c_n);
			c_n = save;
		}

		//std::cout << "Vector size: " << boundary.size() <<
		//	" Last Found " <<
		//	boundary.back().x << " " << boundary.back().y << std::endl;
	}

	// Normalize all of the points so that there's a 1 px buffer around the shape
	for ( unsigned int i = boundary.size(); i-- > 0; )
	{
		boundary[i].x = boundary[i].x - min_x + 1;
		boundary[i].y = boundary[i].y - min_y + 1;
	}

	max_x = max_x - min_x + 1;
	max_y = max_y - min_y + 1;
							
}

//! to_mat generates a normalized mat with our boundary in it.
Mat shape2D::to_mat() {
	Mat output = Mat::zeros(max_y + 2, max_x + 2, CV_8UC1);
	for ( unsigned int i = boundary.size(); i-- > 0; )
	{
		output.at<uchar>(boundary[i]) = 255;
	}

	return output;
}

unsigned int shape2D::getBoundSize() {
	return this->boundary.size();
}



void shape2D::update_extrema()
{

	for(auto p : boundary)
	{
		if ( p.x < min_x )
			min_x = p.x;
		if ( p.y < min_y )
			min_y = p.y;
		if ( p.x > max_x )
			max_x = p.x;
		if ( p.y > max_y )
			max_y = p.y;
	}
		
}

//! shape2D destructor
shape2D::~shape2D()
{
	return;
}
