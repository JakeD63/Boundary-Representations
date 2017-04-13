#include <opencv2/opencv.hpp>
#include <complex>
#include <vector>
#include <iostream>
#include "fourier_desc.hpp"
#include "shape2d.hpp"

using namespace cv;

FourierDescriptor::FourierDescriptor(shape2D& shape)
{
	int bsize =  shape.size();
	int osize =  getOptimalDFTSize( bsize );
	std::vector<std::complex<double> > cmplx;
	for( int i=0; i < osize; i++ )
	{
		if ( i < bsize )
		{
			cmplx.push_back( std::complex<double>(shape[i].x, shape[i].y) );
		} else {
			cmplx.push_back( std::complex<double>(0.0, 0.0) );
		}

		std::cout << cmplx[i].real() << " " << cmplx[i].imag() << std::endl;
	}


	std::vector<std::complex<double> > out;

	dft( cmplx, out );

	for( int i=0; i < out.size(); i++)
	{
		std::cout << "Output real: " << out[i].real()
			<< " imaginary: " << out[i].imag() << std::endl;
	}

	return;	
			
}

Mat FourierDescriptor::to_mat()
{
	/* Perform IDFT */

	/* Translate to X,Y coords */

	/* Plot points onto map */

	/* return map */
}

FourierDescriptor::~FourierDescriptor()
{
	return;
}
