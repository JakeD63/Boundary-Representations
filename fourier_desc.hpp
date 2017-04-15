#ifndef __FOURIER_DESC_HPP
#define __FOURIER_DESC_HPP

#include <opencv2/opencv.hpp>
#include <fftw3.h>
#include "shape2d.hpp"

class FourierDescriptor : public shape2D
{
	private:
		unsigned int N;
		fftw_complex *in, *out;
		fftw_plan fft_plan;
		fftw_complex *ifft;
		

	public:
		FourierDescriptor(cv::Mat img);
		~FourierDescriptor();

		void reconstruct();
};

#endif
