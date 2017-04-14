#ifndef __FOURIER_DESC_HPP
#define __FOURIER_DESC_HPP
#include "shape2d.hpp"
#include <vector>
#include <complex>

class FourierDescriptor {
	
	private:

		cv::Mat desc;

	public:
		FourierDescriptor(shape2D& shape);
		~FourierDescriptor();

		shape2D to_shape2D();
		void set_desc_count(int count);
};

#endif
