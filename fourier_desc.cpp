/**
 * @authors Lawrence Hoffman
 * @brief Implementation of Fourier Descriptors
 *
 * @paragraph Description
 * Class: Digital Image Processing <SPRING 2017>
 * Instructor: Weiss, John PhD
 * Implements Fourier Descriptors of shapes using FFTW and OpenCV
 * 
 * @paragraph DevNotes
 * The purpose of this code is to show how to create Fourier Descriptors as
 * explicitly as possible. This means a few things:
 * 	- code is over commented and assumes a reader unfamiliar with FD
 * 	- code uses more memory than necessary
 * 	- code is written for clarity as opposed to efficiency
 * 	- code contains NO fancy tricks
 */

#include <opencv2/opencv.hpp>
#include <fftw3.h>
#include <cmath>
#include <vector>
#include <limits>
#include "fourier_desc.hpp"

/**
 * Construct a FourierDescriptor object
 *
 * Construct a shape2D then use the functions present in shape2D to build the 
 * base FFT from the boundary values.
 *
 * @return FourierDescriptor object
 */
FourierDescriptor::FourierDescriptor(cv::Mat img) 
	: shape2D(img)
{
	// Set N to be the count of points in the boundary of the underlying
	// shape2D object.
	N = boundary.size();

	// Scale the values headed into the FFT by 1/N, this is necessary because
	// FFTW does not autoscale
	double scale = 1.0 / N;

	// Obtain memory for input array, free in the destructor 
	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

	// Obtain memory for the original boundary, incase we need to recover it
	// free in the destructor
	orig = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
	
	// Obtain memory for output array, free	in the destructor
	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

	// Walk the boundary, scale the values into the input array
	for( int i=0; i < N; i++ )
	{

		// Generate our input array
		in[i][0] = (double) boundary[i].x * scale;
		in[i][1] = (double) boundary[i].y * scale;
	}

	// Generate our FFT plan, free in the destructor
	plan = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

	// Run DFT on the inputs	
	fftw_execute(plan);

	for( int i=0; i < N; i++)
	{
		// Copy to our save array, in case we want to grow the discriptor
		// count and rescue previously destroyed information
		orig[i][0] = out[i][0];
		orig[i][1] = out[i][1];
	}

	// Obtain memory for the ifft_plan
	ifft_plan = fftw_plan_dft_1d(N, out, in, FFTW_BACKWARD, FFTW_ESTIMATE);

	return;	
}

/**
 * Turn whatever is in the output array back into a boundary.
 *
 * @paragraph DevNotes
 * This function runs our inverse fft on whatever is in out, and deposits it
 * into in. It is necessary to make a few corrections for the negative numbers
 * that will be output by the inverse. I've tried to implement this in a way
 * that is obvious. In a production implementation we'd avoid repeatedly looping
 * over the list in order to update the extrema.
 */
void FourierDescriptor::reconstruct()
{
	// set up a flag to track if we need to run a loop correcting neg
	// numbers (we probably will).
	bool ltz_flag = false;	
	
	// Run the plan backwards, outputing into our original in the new 
	// complex values for the boundary	
	fftw_execute(ifft_plan);

	// Throw out the old boundary data
	boundary.clear();

	// Walk the output of our inverse fft and push all of the elements
	// back into the boundary	
	for(int i=0; i < N; i++)
	{

		// A problem I've seen in some peoples videos is a fuzzyness
		// at edges. This is due to bad rounding (or simply explicit
		// casting) note that we want to actually round here.
		int xv = (int) floor(in[i][0] + 0.5);
		int yv = (int) floor(in[i][1] + 0.5);
		boundary.push_back(cv::Point(xv, yv));

		// If either the x or y value is negative, throw ltz_flag
		if( !ltz_flag && xv < 0 || yv < 0)
			ltz_flag = true;

	}

	// Update all of our max and min values, in production code we'd want
	// to handle this in the loop above.
	update_extrema();

	// If any of our values were less than zero
	if( ltz_flag )
	{

		// Move the entire boundary by an amount the accounts
		// for the extreme values
		for(int i=0; i < N; i++) 
		{
			boundary[i].x -= min_x;
			boundary[i].y -= min_y;
		}
		
		// Store the new, correct extrema values
		update_extrema();
	}


}

/**
 * Reconstruct the shape by degrees of Fourier Descriptor.
 *
 * @paragraph Description
 * This function cuts down the number of descriptors used by zeroing out the
 * high frequencies to leave a count of descriptors that matches the degree
 * passed to the function.
 */
void FourierDescriptor::reconstruct(unsigned int degree)
{
	// We cannot have more descriptors than points in the original boundary
	if(degree > N)
	{
		std::cerr << "Max Descriptors(" << N << ") exceeded." 
			<< std::endl;
		return;
	}

	// If we've got a request for more descriptors than used last time we'll
	// need to recover the result of the original DFT from orig.
	if( degree > cur_degree )
	{
		for(int i=0; i < N; i++)
		{
			out[i][0] = orig[i][0];
			out[i][1] = orig[i][1];
		}

	}

	// Keep trag of how many descriptors remain
	cur_degree = degree;

	// Calculate how many descriptors we want zeroed out
	int elim = N - degree;

	// Calculate the start offset in the array
	int start = (int) floor(degree / 2);
	 // start = ((int) floor(N/2)) - start;

	// Calculate how far to walk in the array
	int end = start + elim;

	// Walk from start to end zeroing out the array
	for( int s = start; s < end; s++)
	{
		out[s][0] = 0.0;
		out[s][1] = 0.0;
	}

	// Call reconstruct on our now output array
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


/**
 * Close out allocated resources for FD
 */
FourierDescriptor::~FourierDescriptor()
{	
	
	fftw_free(orig); 
	fftw_free(out);
	fftw_free(in);
	fftw_destroy_plan(ifft_plan);
	fftw_destroy_plan(plan);
	/* return; */

}

