#ifndef __SHAPE2D_HPP
#define __SHAPE2D_HPP

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
/*! shape2D constructs a boundary vector of passed in binary image */
class shape2D
{
private:

    cv::Point find_tm_lm(cv::Mat img);
    void rotate_cw(const cv::Point p, cv::Point& c_n);
    void find_next(cv::Point& p, cv::Point& c_n);

protected:
    void update_extrema();
    std::vector<cv::Point> boundary; /*!< boundary of image */
    int max_x, /*!< max x point */
        max_y, /*!< max y point */
        min_x, /*!< min x point */
        min_y; /*!< min y point */
    cv::Size imgSize; /*!< size of image */

public:
    shape2D(cv::Mat img);
    ~shape2D();

    virtual cv::Mat to_mat();
    unsigned int getBoundSize();
};

#endif
