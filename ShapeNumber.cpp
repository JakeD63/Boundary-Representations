#include "ShapeNumber.hpp"

/**
 * \brief Construct ShapeNumber Object
 *
 * Take in image and scale of grid and use it to construct object.
 * After getting boundary from shape2D, scale it, and normalize the code.
 */
ShapeNumber::ShapeNumber(cv::Mat img, int scale) : shape2D(img)
{
    this->setup(scale);
}

/**
 * \brief Rescale boundary size. This is implemented so you can
 * resize the grid instead of having to create a new object.
 *
 * Clears object data and re-does the constructor
 */
void ShapeNumber::rescaleBoundary(int scale)
{
    this->scaledBoundary.clear();
    this->shapeNumber.clear();
    this->setup(scale);

}

void ShapeNumber::setup(int scale)
{
    setGridScale(scale); //ensure the grid is not too large
    scaleBoundary(); //scale the boundary
    genChainCode(); //generate chain code based on scaled boundary
    normalizeRot(); //normalize for rotation
    getMinMagnitude(); //normalize for starting point
}

/**
 * \brief Ensure the grid scale is within range
 *
 * Check scale against the max grid value.
 * This is done because too large a grid makes
 * the chain code too small to use or display
 */
void ShapeNumber::setGridScale(int scale)
{
    int max = this->GRID_MAX;
    if(scale > max)
        this->gridScale = max;
    else if(scale == 0)
        this->gridScale = 1;
    else
        this->gridScale = scale;
}

/**
 * \brief returns the max scale
 *
 * Returns the max grid scale the object will allow. This is
 * mostly used for the gui trackbar max value.
 */
int ShapeNumber::getMaxGridScale()
{
    return this->GRID_MAX;
}

/**
 * \brief Scale image boundary to new grid size
 *
 * We make the grid larger than 1x1, then calculate
 * the closest grid corner from each point.
 * EX. (12, 33) with a scale of 5 becomes (10, 35).
 * This is done to make similar shapes have similar codes
 */
void ShapeNumber::scaleBoundary()
{
    Point c, tl, tr, bl, br;
    vector<double> distances; //tl, tr, bl, br
    vector<Point> temp_bound;
    int min_p;
    //walk every point in boundary, calculating nearest grid corner
    for (unsigned int i = 0; i < this->boundary.size(); i++) {
        c = this->boundary.at(i);
        //upper left
        tl.x = roundDown(c.x, gridScale);
        tl.y = roundUp(c.y, gridScale);
        //upper right
        tr.x = roundUp(c.x, gridScale);
        tr.y = roundUp(c.y, gridScale);
        //bottom left
        bl.x = roundDown(c.x, gridScale);
        bl.y = roundDown(c.y, gridScale);
        //bottom right
        br.x = roundUp(c.x, gridScale);
        br.y = roundDown(c.y, gridScale);

        //get min distance
        distances.push_back(distance(c, tl));
        distances.push_back(distance(c, tr));
        distances.push_back(distance(c, bl));
        distances.push_back(distance(c, br));
        //get the min distance
        min_p = (int) std::distance(distances.begin(), min_element(distances.begin(), distances.end()));

        //use min distance to get point to add to boundary
        switch (min_p) {
        case 0:
            temp_bound.push_back(tl);
            break;
        case 1:
            temp_bound.push_back(tr);
            break;
        case 2:
            temp_bound.push_back(bl);
            break;
        case 3:
            temp_bound.push_back(br);
            break;
        default:
            cerr << "scaleBoundary: min_p not in range 0-3: " << min_p << endl;
            exit(0);
        }
        //clear distances vector
        distances.clear();
    }
    //add unique points to scaled boundary
    for (unsigned int i = 0; i < temp_bound.size() - 1; i++) {
        if (!(temp_bound.at(i) == temp_bound.at(i + 1)))
            scaledBoundary.push_back(temp_bound[i]);
    }

}

/**
 * \brief Generate a chain code from boundary
 *
 * Walk the boundary storing the direction from
 * the current point to the next. Directions are represented as
 * 0-7 counterclockwise, with 0 being ->
 */
void ShapeNumber::genChainCode()
{
    Point c, n;
    int xDiff, yDiff, code = 0;
    //walk the boundary storing directions
    for (unsigned int i = 0; i < scaledBoundary.size() - 1; i++) {
        //get differences in x and y from current and next point
        c = scaledBoundary.at(i);
        n = scaledBoundary.at(i + 1);
        xDiff = n.x - c.x;
        yDiff = n.y - c.y;
        //use those differences to set direction
        if (xDiff == gridScale) {
            if (yDiff == gridScale)
                code = 1;
            else if (yDiff == 0)
                code = 0;
            else if (yDiff == -gridScale)
                code = 7;
        } else if (xDiff == 0) {
            if (yDiff == gridScale)
                code = 2;
            else if (yDiff == -gridScale)
                code = 6;
        } else if (xDiff == -gridScale) {
            if (yDiff == gridScale)
                code = 3;
            else if (yDiff == 0)
                code = 4;
            else if (yDiff == -gridScale)
                code = 5;
        }
        shapeNumber.push_back(code);
    }
}

/**
 * \brief Gets minimum magnitude of chain code
 *
 * Rotate the chain code until we get one that is smaller than
 * the rest if they are treated as integers
 */
void ShapeNumber::getMinMagnitude()
{
    vector<int> min = shapeNumber; //set starting min
    vector<int> curRot = shapeNumber; //current rotation
    //rotate n times
    for(int i = 0; i < shapeNumber.size(); i++) {
        //store min rotation
        if(compareCodes(curRot, min) == -1)
            min = curRot;
        //rotate again
        rotate(curRot.begin(), curRot.end()-1, curRot.end());
    }
    this->shapeNumber = min;
}

/**
 * \brief Compare the integer value of two codes.
 * Used for normalization
 *
 * Walks both vectors checking for differences
 * Returns 1 for larger, -1 for less than, 0 for equal
 */
int ShapeNumber::compareCodes(vector<int> a, vector<int> b)
{
    //to keep things simple for our purposes, only compare vectors of the same size
    if(a.size() != b.size()) {
        cerr << "compareCodes: vector sizes must be equal" << endl;
        exit(0);
    }
    //walk a and b until numbers are different, then return a<b
    for(int i = 0; i < a.size(); i++) {
        if(a[i] < b[i])
            return -1;
        else if(a[i] > b[i])
            return 1;
    }
    return 0;
}

/**
 * \brief Normalize code for rotation
 *
 * Store difference between rotations in code.
 * This makes the code rotation invariant, so it can
 * more easily be used and compared to other codes.
 */
void ShapeNumber::normalizeRot()
{
    vector<int> tempCode;
    int r;
    for (unsigned int i = 0; i < this->shapeNumber.size() - 1; i++) {
        //difference in directions is p[i + 1] - p[i]
        //if negative, add 8
        r = this->shapeNumber.at(i + 1) - this->shapeNumber.at(i);
        if (r < 0)
            r += 8;
        tempCode.push_back(r);

    }
    this->shapeNumber = tempCode;
}

/**
 * \brief Turns scaled boundary into mat image for display
 *
 * redraws points onto img and returns it
 */
Mat ShapeNumber::to_mat()
{
    this->img = redrawPoints(this->img);
    return this->img;
}

/**
 * \brief Turns scaled boundary into mat image for display.
 * The image has its points connected
 * */
Mat ShapeNumber::to_connected_mat()
{
    //use cvLine to draw lines between all points
    this->connectedImg = this->redrawPoints(this->connectedImg);
    for(int i = 0; i < scaledBoundary.size() - 1; i++) {
        line(this->connectedImg, scaledBoundary.at(i), scaledBoundary.at(i+1), Scalar(255,255,255),1,CV_AA );
    }
    return this->connectedImg;
}

/**
 * \brief Draws scaled boundary onto image
 *
 * Walks boundary drawing points from scaled boundary.
 * NOTE: the = operator is overloaded for Mat images of the same size,
 * it overwrites img instead of construcing a new one. This means it is more
 * memory efficient than constructing a new image.
 */
Mat ShapeNumber::redrawPoints(Mat img)
{
    img = Mat::zeros(max_y + 2, max_x + 2, CV_8UC1);
    for(unsigned int i = 0; i < scaledBoundary.size(); i++) {
        img.at<uchar>(scaledBoundary[i]) = 255;
    }
    return img;
}

/**
 * \brief Returns the shape number vector
 */
vector<int> ShapeNumber::getCode()
{
    return this->shapeNumber;
}

/**
 * \brief Get specific number from code
 */
int ShapeNumber::at(unsigned int i)
{
    return this->shapeNumber.at(i);
}

/**
 * \brief Get the size of the shape number
 */
int ShapeNumber::size()
{
    return (int) this->shapeNumber.size();
}

/**
 * \brief Calculate the distance between two points
*/
double ShapeNumber::distance(Point a, Point b)
{
    return sqrt(pow((b.x - a.x), 2) + pow((b.y - a.y), 2));
}

/**
 * \brief Rounds n up to nearest multiple of m
 *
 * This is used for grid rescaling.
 * @param n number to round
 * @param m multiple to round to
 */
int ShapeNumber::roundUp(int n, int m)
{
    int r;
    //if multiple is 0, return 0
    if (m == 0)
        return n;
    //if n is a multiple of m, return 0
    r = n % m;
    if (r == 0)
        return n;
    //return n + m - remainder
    return n + m - r;
}

/**
 * \brief rounds down using roundUp function
 */
int ShapeNumber::roundDown(int n, int m)
{
    return roundUp(n - m, m);
}

/**
 * \brief Overloaded print operator for shape number
 */
ostream &operator<<(ostream &os, const ShapeNumber &s)
{
    for (auto i : s.shapeNumber)
        os << i;
    return os;
}

/**
 * \brief Overloaded [] operator for shape number vector
 */
int& ShapeNumber::operator[](unsigned int i)
{
    return this->shapeNumber.at(i);
}