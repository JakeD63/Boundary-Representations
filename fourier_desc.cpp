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
#include <string.h>
#include <fftw3.h>
#include <cmath>
#include <vector>
#include "fourier_desc.hpp"

FourierDescriptor::FourierDescriptor(cv::Mat img) 
	: shape2D(img)
{
	N = boundary.size();

	desc = N;

	double scale = 1.0 / N;

	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

	for( int i=0; i < N; i++ )
	{
		in[i][0] = (double) boundary[i].x * scale;
		in[i][1] = (double) boundary[i].y * scale;
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
	ifft_plan = fftw_plan_dft_1d(desc, out, ifft, FFTW_BACKWARD, FFTW_ESTIMATE);
	
	double scale = 1.0;
	fftw_execute(ifft_plan);

	std::vector<cv::Point> output;
	
	for(int i=0; i < desc; i++)
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

		std::cout << "Number: " << i << "\t" << output[i] << std::endl;

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

		for(int i=0; i < desc; i++) 
		{
			boundary[i].x -= old_min_x;
			boundary[i].y -= old_min_y;
			std::cout << boundary[i] << std::endl;
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

cv::Mat FourierDescriptor::to_mat()
{
	std::cout << "Max x: " << max_x << "Max y: " << max_y << std::endl;
	cv::Mat out_mat = cv::Mat::zeros(max_y + 2, max_x + 2, CV_8UC1);
	for ( unsigned int i = boundary.size(); i-- > 0; )
	{

		std::cout << "Counting Down: " << i << std::endl;
		out_mat.at<uchar>(boundary[i]) = 255;
	}
	return out_mat;

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
	
	memset(out, 0, N);	
	int center = (int) floor(N/2);
	int start = (int) floor(center/2);

	memcpy(res, ifft+(start), sizeof(fftw_complex)*degree);

	ifftshift(res, out, degree);
	
	fftw_free(ifft);

	desc = degree;

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
