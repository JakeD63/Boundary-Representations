#ifndef DIP3_CHAINCODE_H
#define DIP3_CHAINCODE_H
#include "shape2d.hpp"

using namespace std;
using namespace cv;

/*! ShapeNumber calculates the shape number a boundary that has been rescaled to a different grid size */
class ShapeNumber : public shape2D {
public:
	ShapeNumber(Mat img, int scale = 1);
	void rescaleBoundary(int scale);
	Mat to_mat();
	Mat to_connected_mat();
	int getMaxGridScale();
	vector<int> getCode();
	int at(unsigned int i);
	int size();
	friend ostream &operator<<(ostream &os, const ShapeNumber &s);
	int& operator[](unsigned int i);

private:
	void setGridScale(int scale);
	void scaleBoundary();

	void genChainCode();

	void getMinMagnitude();

	void normalizeRot();

	int compareCodes(vector<int> a, vector<int> b);

	double distance(Point a, Point b);

	Mat redrawPoints(Mat img);
	int roundUp(int n, int m);

	int roundDown(int n, int m);

	Mat img, /*!<image to draw points on */
		connectedImg; /*!<image with points connected */
	int gridScale; /*!< scale of grid size (ex, 5x5) */
	const int GRID_MAX = 100; /*!< max grid scale allowed */
	std::vector<int> shapeNumber; /*!< shape number of boundary */
	std::vector<Point> scaledBoundary; /*!< boundary after rescaling of grid */
};


#endif //DIP3_CHAINCODE_H
