#ifndef __FOURIER_DESC_HPP
#define __FOURIER_DESC_HPP

#include <opencv2/opencv.hpp>
#include <fftw3.h>
#include "shape2d.hpp"

class FourierDescriptor : public shape2D
{
	private:
		unsigned int N, desc;
		fftw_complex *in, *out;
		fftw_plan fft_plan;
		fftw_complex *ifft;
		void fftshift(fftw_complex *input, fftw_complex *output, int dim);
		void ifftshift(fftw_complex *input, fftw_complex *output, int dim);
		void truncate(int degree);
	public:
		FourierDescriptor(cv::Mat img);
		~FourierDescriptor();

		void reconstruct();
		void reconstruct(unsigned int degree);
		cv::Mat to_mat();
		
};

#endif
