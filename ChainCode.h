//
// Created by jake on 4/14/17.
//

#ifndef DIP3_CHAINCODE_H
#define DIP3_CHAINCODE_H
#include "shape2d.hpp"

using namespace std;
using namespace cv;
class ChainCode : public shape2D {
    public:
        ChainCode(cv::Mat img);
		vector<int> getCode();
		friend ostream& operator<<(ostream& os, const ChainCode& cc);
    private:
        void genChainCode();
		void normalizeRot();
        std::vector<int> chainCode;

};


#endif //DIP3_CHAINCODE_H
