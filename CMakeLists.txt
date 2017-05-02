cmake_minimum_required(VERSION 2.8)
project( boundaries CXX )
set(CMAKE_CXX_STANDARD 11)
add_compile_options(-std=c++11)

find_package( OpenCV REQUIRED )

find_library( FFTW_LIBRARY
	NAMES fftw3)
set(FFTW_LIBRARIES "${FFTW_LIBRARY}")

include_directories( ${OpenCV_INCLUDE_DIRS} )
add_executable( boundaries
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
	target_link_libraries( boundaries ${OpenCV_LIBS} ${FFTW_LIBRARIES} )