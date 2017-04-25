/**
 * @mainpage Shape Descriptors
 * @authors Jake Davidson, Lawrence Hoffman
 *
 * @section intro_sec Introduction
 * This project was created as a final product for the spring 2017 digital image
 * processing class at SDSM&T. The class was instructed by John Weiss, PhD. This
 * software implements two primary types of shape representation, namely chain
 * codes and Fourier descriptors.
 *
 * @section reqs_sec Requirements
 * This software has three requirements:
 * 	- OpenCV
 * 	- FFTW3
 * 	- CMAKE  >= 2.8
 *
 * @section build_sec Building
 * CMake is utilized to build this project. CMake generates makefiles based on
 * the host system. Follow these steps to build:
 * 	1. Ensure that you have the required libraries and CMake is installed
 * 	2. Create an empty build directory
 * 	3. From within the build directory run `cmake /path/to/src/directory`
 * 	4. From within the build directory run `make`
 *
 * @section usage_sec Usage
 * Once built, run the executable dip3 with one argument, an input image. Note
 * that there are some requirements on the input image. Isolating shapes was
 * not a part of this project, the focus was on creating descriptors of shapes,
 * therefore the input images should be a single shape on a white background.
 *
 * @section impl_notes Implementation Notes
 * The code in this project was written as an educational exercise. The code
 * is verbosely commented. Also, there are many places where more efficient
 * solutions were possible, but the authors chose to write code that was more
 * clear in intent.
 */
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

#include "shape2d.hpp"
#include "fourier_desc.hpp"
#include "ShapeNumber.hpp"

#include "gui.cpp"

using namespace std;
using namespace cv;

const string HELP = "usage: ./dip3 <imagefile>";

/**
 * Simply convert color and threshold the image
 */
void preProcess(Mat bimg, Mat& res)
{
    cvtColor(bimg, bimg, CV_BGR2GRAY);
    threshold(bimg, res, 128, 255, 1);
}

int main(int argc, char **argv)
{
    if ( argc < 2 ) {
        cout << HELP << endl;
        return 1;
    }

    Mat res_img;
    Mat img = imread(argv[1]);
    if( !img.data )
        return 2;

    preProcess(img, res_img);

    // TODO: Insert ShapeD code here


    fd_gui fg(res_img, 600, 600, string("Fourier Descriptors"));
    createTrackbar("Descriptor Count", fg.wName, &fg.desc_count, fg.max_d, showFdGui, &fg);

    shapenumber_gui sg(res_img, 600, 600, string("Shape Number"));
    createTrackbar("Grid Scale", sg.wName, &sg.gridScale, sg.maxScale, showSnGui, &sg);
    waitKey(0);
    return 0;


}

