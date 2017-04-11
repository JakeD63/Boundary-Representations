#ifndef __SHAPE2D_HPP
#define __SHAPE2D_HPP

#include <opencv2/opencv.hpp>
#include <vector>
#include <pair>

class shape2D {

	std::vector<std::pair<unsigned int, unsigned int> > boundary;

	public:
		shape2D(cv::Mat);
		~shape2D();

};

#endif
