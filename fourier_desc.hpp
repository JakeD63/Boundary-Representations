#ifndef __FOURIER_DESC_HPP
#define __FOURIER_DESC_HPP

#include <opencv2/opencv.hpp>
#include <fftw3.h>
#include "shape2d.hpp"

class FourierDescriptor : public shape2D
{
	private:
		unsigned int N, cur_degree;
		fftw_complex *out, *in, *orig;
		fftw_plan plan, ifft_plan;

	public:
		FourierDescriptor(cv::Mat img);
		~FourierDescriptor();

		void reconstruct();
		void reconstruct(unsigned int degree);
		cv::Mat to_mat();
		
};

#endif
