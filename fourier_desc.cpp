#include <opencv2/opencv.hpp>
#include <fftw3.h>
#include <cmath>
#include <vector>
#include "fourier_desc.hpp"

FourierDescriptor::FourierDescriptor(cv::Mat img) 
	: shape2D(img)
{
	N = boundary.size();

	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

	for( int i=0; i < N; i++ )
	{
		in[i][0] = (double) boundary[i].x;
		in[i][1] = (double) boundary[i].y;
	}

	fft_plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	
	fftw_execute(fft_plan);


	return;	
}

FourierDescriptor::~FourierDescriptor()
{
	fftw_free(ifft);

	fftw_destroy_plan(fft_plan);
	fftw_free(in);
	fftw_free(out);
	return;
}

void FourierDescriptor::reconstruct()
{
	fftw_plan ifft_plan;
	ifft = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
	ifft_plan = fftw_plan_dft_1d(N, out, ifft, FFTW_BACKWARD, FFTW_ESTIMATE);
	double scale = 1.0 / N;
	fftw_execute(ifft_plan);

	std::vector<cv::Point> output;

	std::cout << "Ox:\tOy:\tIx:\tIy:\t" << std::endl;
	for(int i=0; i < N; i++)
	{
		unsigned int xv = (unsigned int) floor(scale * ifft[i][0] + 0.5);
		unsigned int yv = (unsigned int) floor(scale * ifft[i][1] + 0.5);
		output.push_back(cv::Point(xv, yv));

		std::cout << boundary[i].x << "\t" <<  boundary[i].y 
		       << "\t" << xv << "\t" << yv << std::endl;
	}

	fftw_destroy_plan(ifft_plan);	
}

