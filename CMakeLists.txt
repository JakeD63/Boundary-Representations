cmake_minimum_required(VERSION 2.8)
project( dip3 CXX )
set(CMAKE_CXX_STANDARD 11)

find_package( OpenCV REQUIRED )

find_library( FFTW_LIBRARY
	NAMES fftw3 fftw)
set(FFTW_LIBRARIES "${FFTW_LIBRARY}")

include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( dip3 
	main.cpp 
	shape2d.hpp
	shape2d.cpp
	ShapeNumber.cpp 
	ShapeNumber.hpp 
	ChainCode.cpp 
	ChainCode.hpp
	fourier_desc.hpp
	fourier_desc.cpp
	)
	target_link_libraries( dip3 ${OpenCV_LIBS} ${FFTW_LIBRARIES} )