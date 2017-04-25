#ifndef __FOURIER_DESC_HPP
#define __FOURIER_DESC_HPP

#include <fftw3.h>
#include "shape2d.hpp"

/*! Class to calculate and output Fourier Descriptors using boundary from shape2D */
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
		int size();
		cv::Mat to_mat();

};

#endif
