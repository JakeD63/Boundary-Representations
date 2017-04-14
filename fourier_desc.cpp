#include <opencv2/opencv.hpp>
#include <complex>
#include <cmath>
#include <vector>
#include <iostream>
#include "fourier_desc.hpp"
#include "shape2d.hpp"
using namespace cv;

FourierDescriptor::FourierDescriptor(shape2D& shape)
{

	std::cout << "Start fd" << std::endl;	
	auto bnd = shape.get_boundary();
	std::vector<std::complex<double> > cmplx;

	std::cout << "Building cmplx" << std::endl;
	for( Point p : bnd )
	{
		cmplx.push_back(std::complex<double>(p.x, p.y));
	}
	
	std::cout << "Building mats" << std::endl;	
	Mat in_mat(cmplx);
	Mat fd(in_mat.size(), DataType<std::complex<double> >::type);

	std::cout << "Forward DFT" << std::endl;
	dft(in_mat, fd);

	desc = fd.clone();
	return;	
			
}

void FourierDescriptor::set_desc_count(int count)
{
	if ( count > desc.rows )
	{
		std::cerr << "Cannot grow the descriptor count" << std::endl;
		return;
	}

	for(int i=count; i < desc.rows; i++)
	{
		desc.row(i).setTo(Scalar(0.0));
	}	

}

shape2D FourierDescriptor::to_shape2D()
{

	std::cout << "Begin IDFT fd info " << desc.rows << std::endl;
	std::cout << desc << std::endl;
	Mat rev_mat(desc.size(), DataType<std::complex<double> >::type);
	dft(desc, rev_mat, DFT_INVERSE + DFT_SCALE);
	std::vector<Point> result;

	Mat real, imag;
	std::vector<Mat> channels(2);
	split(rev_mat, channels);
	real = channels[0];
	imag = channels[1];

	for(int i=0; i < real.rows; i++)
	{
		unsigned int xv = (unsigned int) floor( real.at<double>(i) + 0.5 );
		unsigned int yv = (unsigned int) floor( imag.at<double>(i) + 0.5 );

		result.push_back(Point(xv, yv));

	}
	

	shape2D rshape(result);
	return rshape;
}

FourierDescriptor::~FourierDescriptor()
{
	return;
}
