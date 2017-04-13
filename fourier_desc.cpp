#include <opencv2/opencv.hpp>
#include <complex>
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
	
	std::cout << "Cmplx built" << std::endl;
	auto optSz = getOptimalDFTSize( bnd.size() );

	std::cout << "Padding" << std::endl;	
	for( int i = optSz - bnd.size(); i > 0; i-- )
		cmplx.push_back(std::complex<double>(0, 0));		
	
	std::cout << "Building mats" << std::endl;	
	Mat in_mat(cmplx);
	Mat out_mat(in_mat.size(), DataType<std::complex<double> >::type);
	Mat rev_mat(in_mat.size(), DataType<std::complex<double> >::type);

	std::cout << "Forward DFT" << std::endl;
	dft(in_mat, out_mat);

	std::cout << "IDFT" << std::endl;
	dft(out_mat, rev_mat, DFT_INVERSE + DFT_SCALE); 
	
	std::vector<Point> result;

	Mat real, imag;
	std::vector<Mat> channels(2);
	split(rev_mat, channels);
	real = channels[0];
	imag = channels[1];

	for(int i=0; i < bnd.size(); i++)
	{
		std::cout << "Original (" << bnd[i].x << " , " << bnd[i].y << ") New ("
			<< real.at<double>(i) << " , " << imag.at<double>(i) << ") " << std::endl;
	}

	std::cout << "Done." << std::endl;
	return;	
			
}

shape2D FourierDescriptor::to_mat()
{
	std::vector<Point> p;
	return shape2D(p);
}

FourierDescriptor::~FourierDescriptor()
{
	return;
}
