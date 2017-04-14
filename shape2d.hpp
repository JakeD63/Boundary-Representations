#ifndef __SHAPE2D_HPP
#define __SHAPE2D_HPP

#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

class shape2D {
	private:
		unsigned int max_x, max_y, min_x, min_y;
		cv::Point find_tm_lm(cv::Mat img);
		void rotate_cw(const cv::Point p, cv::Point& c_n);
		void find_next(cv::Point& p, cv::Point& c_n);

	protected:
		std::vector<cv::Point> boundary;

	public:
		shape2D(cv::Mat img);
		~shape2D();

		cv::Mat to_mat();
};

#endif
