#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "constant.hpp"

using namespace cv;
using namespace std;

class Detector {
   private:
    int redCircleCounter  = 0;
    int blueSquareCounter = 0;

    Mat image;

    Mat findRedColor(Mat, Mat);
    Mat findBlueColor(Mat, Mat);
    bool isSquare(vector<Point> &);
    bool isCircle(vector<Point> &, vector<Point> &);
    bool isRectangle(vector<Point> &, vector<Point> &);
    void mPutText(Mat, string text, Point point, Scalar color);

   public:
    Detector();
    ~Detector();

    Bundle process(Mat, Point2d);
};

#endif /* DETECTOR_HPP */
