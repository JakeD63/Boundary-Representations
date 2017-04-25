#ifndef __FOURIER_DESC_HPP
#define __FOURIER_DESC_HPP

#include <fftw3.h>
#include "shape2d.hpp"

/*! Class to calculate and output Fourier Descriptors using boundary from shape2D */
class FourierDescriptor : public shape2D
{
private:
    unsigned int N;    		/*< size of the problem */
    unsigned int cur_degree;	/*< current descriptor count */

    fftw_complex *in; 		/*< complex input */
    fftw_complex *orig;		/*< output with all descriptors */
    fftw_complex *out;		/*< output with cur_degree descriptors */

    fftw_plan plan;			/*< forward plan */
    fftw_plan ifft_plan;		/*< inverse plan */

public:
    FourierDescriptor(cv::Mat img); /*< build FD from cv::Mat */
    ~FourierDescriptor();		/*< free fftw resources */

    void reconstruct();		/*< rebuild all descriptors */
    void reconstruct(unsigned int degree); /*< rebuild degree descriptors */
    int size(); 			/*< get boundary size */
    cv::Mat to_mat();		/*< return an image */

};

#endif
