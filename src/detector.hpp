#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

using namespace cv;
using namespace std;

enum Types { RECT, SQUARE, CIRCLE };

struct Bundle {
    Types type;
    Point positon;
    vector<Point> approx;
};

struct Bound {
    Scalar lower;
    Scalar upper;
};

class Detector {
   private:
    const int MIN_AREA               = 1e3;
    const int APPROX_SIZE_RECT       = 4;
    const int MIN_APPROX_SIZE_CIRCLE = 6;

    const Bound BOUNDS_OF_BLUE      = {Scalar(90, 150, 30), Scalar(130, 255, 255)};
    const Bound BOUNDS_OF_RED_LEFT  = {Scalar(0, 100, 100), Scalar(10, 255, 255)};
    const Bound BOUNDS_OF_RED_RIGHT = {Scalar(160, 100, 100), Scalar(180, 255, 255)};

    Mat findRedColor(Mat, Mat);
    Mat findBlueColor(Mat, Mat);

    bool isSquare(vector<Point> &);
    bool isCircle(vector<Point> &, vector<Point> &);
    bool isRectangle(vector<Point> &, vector<Point> &);

   public:
    Detector();
    ~Detector();

    vector<Bundle> process(Mat);
};

#endif