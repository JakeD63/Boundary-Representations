#ifndef __SHAPE2D_HPP
#define __SHAPE2D_HPP

#include <opencv2/opencv.hpp>
#include <vector>

class shape2D {
	private:
		cv::Point find_tm_lm(cv::Mat img);
		void rotate_cw(const cv::Point p, cv::Point& c_n);
		void find_next(cv::Point& p, cv::Point& c_n);
		std::vector<cv::Point> boundary;

	public:
		shape2D(cv::Mat img);
		~shape2D();

};

#endif
