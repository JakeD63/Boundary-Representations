#ifndef __FOURIER_DESC_HPP
#define __FOURIER_DESC_HPP
#include "shape2d.hpp"
#include <vector>
#include <complex>

class FourierDescriptor {
	
	private:

		cv::Mat fd;
		cv::Mat pt;

	public:
		FourierDescriptor(shape2D& shape);
		~FourierDescriptor();

		shape2D to_mat();
};

#endif
