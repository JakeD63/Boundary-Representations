/**
 * @authors Lawrence Hoffman
 * @brief Implementation of Fourier Descriptors
 *
 * @paragraph description
 * Class: Digital Image Processing <SPRING 2017>
 * Instructor: Weiss, John PhD
 * Implements Fourier Descriptors of shapes using FFTW and OpenCV
 */

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

	fftw_destroy_plan(fft_plan);
	fftw_free(in);
	fftw_free(out);
	return;
}

void FourierDescriptor::reconstruct()
{
	fftw_plan ifft_plan;
	fftw_complex* ifft = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
	ifft_plan = fftw_plan_dft_1d(N, out, ifft, FFTW_BACKWARD, FFTW_ESTIMATE);
	
	double scale = 1.0 / N;
	fftw_execute(ifft_plan);

	std::vector<cv::Point> output;

	std::cout << "Count:\tOx:\tOy:\tIx:\tIy:\t" << std::endl;
	for(int i=0; i < N; i++)
	{
		unsigned int xv = (unsigned int) floor(scale * ifft[i][0] + 0.5);
		unsigned int yv = (unsigned int) floor(scale * ifft[i][1] + 0.5);
		output.push_back(cv::Point(xv, yv));

		std::cout << i << "\t" <<  boundary[i].x << "\t" <<  boundary[i].y 
		       << "\t" << xv << "\t" << yv << std::endl;
	}

	fftw_destroy_plan(ifft_plan);	
	fftw_free(ifft);
}

void FourierDescriptor::reconstruct(unsigned int degree)
{
	truncate(degree);
	reconstruct();
}

void FourierDescriptor::truncate(int degree)
{
	fftw_complex* ifft = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
	fftw_complex* res = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * degree);	
	
	fftshift(out, ifft, N); 

	// Remove N - degree / 2 terms from each side
		
	fftw_free(out);
	fftw_complex* out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * degree);	
	int center = (int) floor(N/2);
	int start = (int) floor(center/2);

	memcpy(res, ifft+(start), sizeof(fftw_complex)*degree);

	ifftshift(res, out, degree);
	
	fftw_free(ifft);
	N = degree;

}

void FourierDescriptor::fftshift(fftw_complex *input, 
		fftw_complex *output, int dim)
{
	int pivot = (dim % 2 == 0) ? (dim / 2) : ((dim - 1) / 2);
	int rh = dim - pivot;
	int lh = pivot;
	memcpy(output, input+(pivot), sizeof(fftw_complex)*rh);
	memcpy(output+rh, input, sizeof(fftw_complex)*lh);	
}	

void FourierDescriptor::ifftshift(fftw_complex *input, 
		fftw_complex *output, int dim)
{
	int pivot = (dim % 2 == 0) ? (dim / 2) : ((dim + 1) / 2);
	int rh = dim - pivot;
	int lh = pivot;
	memcpy(output, input+(pivot), sizeof(fftw_complex)*rh);
	memcpy(output+rh, input, sizeof(fftw_complex)*lh);	

}
