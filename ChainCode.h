//
// Created by jake on 4/14/17.
//

#ifndef DIP3_CHAINCODE_H
#define DIP3_CHAINCODE_H
#include "shape2d.hpp"

class ChainCode : shape2D {
    public:
        ChainCode(cv::Mat img): shape2D(img){};
    private:
        std::vector<int> genChainCode();
        std::vector<int> chainCode;

};


#endif //DIP3_CHAINCODE_H
