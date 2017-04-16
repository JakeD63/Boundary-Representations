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

	double scale = 1.0 / N;

	fftw_complex* in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

	for( int i=0; i < N; i++ )
	{
		in[i][0] = (double) boundary[i].x * scale;
		in[i][1] = (double) boundary[i].y * scale;
	}

	fftw_plan fft_plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	
	fftw_execute(fft_plan);
	fftw_free(in);
	fftw_destroy_plan(fft_plan);

	return;	
}

FourierDescriptor::~FourierDescriptor()
{

	fftw_free(out);
	return;
}

void FourierDescriptor::reconstruct()
{
	fftw_plan ifft_plan;
	fftw_complex* ifft = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
	ifft_plan = fftw_plan_dft_1d(N, out, ifft, FFTW_BACKWARD, FFTW_ESTIMATE);
	
	double scale = 1.0;
	fftw_execute(ifft_plan);

	std::vector<cv::Point> output;
	
	for(int i=0; i < N; i++)
	{
		int xv = (int) floor(scale * ifft[i][0] + 0.5);
		int yv = (int) floor(scale * ifft[i][1] + 0.5);
		output.push_back(cv::Point(xv, yv));
		
		if (xv < min_x)
			min_x = xv;
		if (xv > max_x)
			max_x = xv;
		if (yv < min_y)
			min_y = yv;
		if (yv > max_y)
			max_y = yv;

	}

	boundary = output;

	if( min_x < 0 || min_y < 0 )
	{
		int old_min_x = min_x;
		int old_min_y = min_y;
		min_x = 99999;
		max_x = 0;
		min_y = 99999;
		max_x = 0;

		for(int i=0; i < N; i++) 
		{
			boundary[i].x -= old_min_x;
			boundary[i].y -= old_min_y;
			if(boundary[i].x < min_x)
				min_x = boundary[i].x;
			if(boundary[i].x > max_x)
				max_x = boundary[i].x;
			if(boundary[i].y < min_y)
			        min_y = boundary[i].y;
			if(boundary[i].y > max_y)
				max_y = boundary[i].y;
		}
	}


	fftw_destroy_plan(ifft_plan);	
	fftw_free(ifft);
}

void FourierDescriptor::reconstruct(unsigned int degree)
{
	if(degree > N)
	{
		std::cerr << "Cannot grow descriptor count" << std::endl;
		return;
	}

	int elim = N - degree;
	int start = (int) floor(degree / 2);
	int end = start + elim;
	for( int s = start; s < end; s++)
	{
		out[s][0] = 0.0;
		out[s][1] = 0.0;
	}

	out[0][0] = 0.0;
	out[0][0] = 0.0;

	reconstruct();
}

cv::Mat FourierDescriptor::to_mat()
{
	cv::Mat out_mat = cv::Mat::zeros(max_y + 2, max_x + 2, CV_8UC1);
	for ( unsigned int i = boundary.size(); i-- > 0; )
	{
		out_mat.at<uchar>(boundary[i]) = 255;
	}
	return out_mat;

}


