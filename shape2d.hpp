#ifndef __SHAPE2D_HPP
#define __SHAPE2D_HPP

#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>
#include <iterator>
#include <algorithm>
#include <string>

class shape2D {
	private:
<<<<<<< HEAD
		cv::Point find_tm_lm(cv::Mat img);
		void rotate_cw(const cv::Point p, cv::Point& c_n);
		void find_next(cv::Point& p, cv::Point& c_n);

	protected:
		std::vector<cv::Point> boundary;
		unsigned int max_x, max_y, min_x, min_y;

=======
		
		cv::Point find_tm_lm(cv::Mat img);
		void rotate_cw(const cv::Point p, cv::Point& c_n);
		void find_next(cv::Point& p, cv::Point& c_n);
	
	protected:
		std::vector<cv::Point> boundary;
		int max_x, max_y, min_x, min_y;
>>>>>>> b521b1ff6c9259bbe50375aeafa5c58486071115

public:
		shape2D(cv::Mat img);
		~shape2D();

		virtual cv::Mat to_mat();
};

#endif
