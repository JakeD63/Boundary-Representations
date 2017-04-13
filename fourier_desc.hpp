#ifndef __FOURIER_DESC_HPP
#define __FOURIER_DESC_HPP
#include "shape2d.hpp"
#include <vector>
#include <complex>

class FourierDescriptor {
	
	private:
		std::vector<std::complex<double> > out;

	public:
		FourierDescriptor(shape2D& shape);
		~FourierDescriptor();

		cv::Mat to_mat();
};

#endif
