# Boundary-Representations

## Description
This program implements two common representations of shape boundaries, fourier descriptors and shape numbers.

### Fourier Descriptors
Fourier descriptors treat the boundary of a shape as a 1D array of complex numbers of the form (x + iy). 
This array is then fed into FFTW3, giving us a frequency array. This array is then zeroed out in the center, 
eliminating high frequencies (user passes in amount to keep). The result, after returning from the DFT, is a 
simplified representation of the shape based on the nunmber of points retianed in the frequency array.

### Shape Numbers
Shape numbers are a way of representing a boundary using the direction from one boundary point to the next. 
We start at a point on the boundary, and keep track of the direction from each pixel to the next pixel, 
storing these values (0-7, 0 being east, 1 being northeast, etc) into a vector. Th vector then gets normalized for
rotation and starting point of the shape. We can obtain different amounts of detail by rescaling the grid the boundary follows,
so for example if we scale to a 5x5 grid then the point 12, 33 would become 10, 35. This allows us to eliminate detail while 
retaining the shape, making it easier to compare with other shape numbers.

### GUI
The program expects images to be only the shape you want, centered on a white(ish) background, as it will walk from 
top most left most point to find the starting point for the boundary. There are sample images in this repository 
(I reccommend chess1, it looks the coolest). When run, two windows will appear, one for the fourier descriptors 
of the shape, and one for the shape number. Each window has a trackbar, the fourier trackbar controls the number 
of points used in the new representation, and the shape number trackbar controls the scale of the grid when 
eliminating detail (i.e 5 = 5x5). Note that most trackbar values for fourier descriptors will show little change,
you will need to scale down to around 50 to see a big difference.

## Installation

### Dependencies
- Cmake (version >= 2.8)
  - https://cmake.org/
- OpenCV
  - http://opencv.org/
- FFTW3
  - http://www.fftw.org/

Many package managers will contain these libraries by default, but we reccommend building from source to get the most 
up to date versions. 

### Build
- git clone repo_url
- cd repo_folder
- mkdir build
- cd build
- cmake ..
- make

### Usage
./boundaries image.jpg

