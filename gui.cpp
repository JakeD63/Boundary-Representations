#include "shape2d.hpp"
#include "ShapeNumber.hpp"
#include <opencv2/opencv.hpp>


struct shapenumber_gui {
    int wHeight, /*!< height of window */
        wWidth, /*!< width of window */
        gridScale = 1, /*!< updated for trackbar position */
        maxScale; /*!< max value for trackbar */
    string wName; /*!< name of window */
    ShapeNumber sn; /*!< shape number object to display */

    /**
    * \brief constructor, set up member varaibles and display initial window
    */
    shapenumber_gui(Mat img, int height, int width, string name) :
        sn(img),
        wName(name),
        wHeight(height),
        wWidth(width)
    {
        //set variables
        maxScale = sn.getMaxGridScale();
        namedWindow(wName);
        resizeWindow(wName, 600, 600);
        //show shape number
        imshow(wName, sn.to_mat());
    }

};

/**
 * \brief callback to display shape number
 */
void showSnGui(int trac, void* data)
{
    shapenumber_gui* temp = (shapenumber_gui*) data;
    temp->sn.rescaleBoundary(trac); //rescale bondary based on trackbar position
    imshow(temp->wName, temp->sn.to_mat());
}

/**
 * @This struct is a way to get out of using global variables to controll the
 * fourier descriptor window
 */
struct fd_gui {
    int desc_count, /*!< number of descriptors, updated on trackbar movement */
        max_d, 	    /*!< max descriptor count for trackbar */
        wHeight,    /*!< height of window */
        wWidth;     /*!< width of window */
    string wName;   /*!< name of window */
    FourierDescriptor fd; /*!< fourier descriptor to display */

    /**
    * @brief constructor, set member variables and display initial window
    */
    fd_gui(Mat img, int height, int width, string name) :
        fd(img),
        wHeight(height),
        wWidth(width),
        wName(name)
    {
        //set variables
        max_d = fd.size();
        desc_count = (int) floor(max_d / 2);
        namedWindow(wName);
        resizeWindow(wName, wWidth, wHeight);

        //display initial window
        imshow(wName, fd.to_mat());
    };

};

/**
 * @brief is the callback function called by the trackbar to update the fourier
 * descriptor window
 */
void showFdGui(int trac, void* data)
{
    fd_gui* temp = (fd_gui*) data;
    temp->fd.reconstruct(trac);
    imshow(temp->wName, temp->fd.to_mat());
}

