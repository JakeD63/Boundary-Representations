#ifndef DIP3_CHAINCODE_HPP
#define DIP3_CHAINCODE_HPP

#include "shape2d.hpp"
using namespace std;
using namespace cv;

/*! Chain Code calculates the chain code of boundary from shape2D */
class ChainCode : public shape2D
{
public:
    ChainCode(Mat img);

    Mat to_mat();
    Mat to_connected_mat();

    vector<int> getCode();
    int at(unsigned int i);
    int size();
    friend ostream &operator<<(ostream &os, const ChainCode &cc);
    int& operator[](unsigned int i);

private:

    void genChainCode();

    void normalizeRot();

    std::vector<int> chainCode; /*!< chain code of boundary */

    Mat img, /*!< image to output */
        connectedImg; /*!< image with connected points */
};


#endif //DIP3_CHAINCODE_HPP
