#pragma once

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>


using namespace cv;
using namespace std;

class Operation {
   private:
    VideoCapture capture;
    Point2d frameSize;
    Point2d centerOfCapture;
    double frameCount;

    void init();
    void process(Mat &);
    void mPutText(Mat &frame, string, Point, Scalar color);

    bool isSquare(vector<Point> &);
    bool isCircle(vector<Point> &, vector<Point> &);
    bool isRectangle(vector<Point> &, vector<Point> &);

    Mat findRedColor(Mat &, Mat &);
    Mat findBlueColor(Mat &, Mat &);

   public:
    Operation(VideoCapture &);
    ~Operation();

    Point2d getFrameSize() {
        return this->frameSize;
    }

    void setFrameSize(Point2d frameSize) {
        this->frameSize = frameSize;
    }

    double getFrameCount() {
        return this->frameCount;
    }

    void setFrameCount(double frameCount) {
        this->frameCount = frameCount;
    }

    Point2d getCenterOfCapture() {
        return this->centerOfCapture;
    }

    void setCenterOfCapture(Point2d centerOfCapture) {
        this->centerOfCapture = centerOfCapture;
    }
};
