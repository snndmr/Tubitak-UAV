#pragma once

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

struct Bound
{
	Scalar lower;
	Scalar upper;
};

class Operation
{
private:
	const float MIN_AREA = 1e3;

	const char *WINDOW_NAME_MAIN = "Window Main";
	const char *WINDOW_NAME_PREP = "Window Prep";

	const Bound redR = {Scalar(0, 100, 100), Scalar(10, 255, 255)};
	const Bound redL = {Scalar(160, 100, 100), Scalar(180, 255, 255)};
	const Bound blue = {Scalar(100, 100, 100), Scalar(130, 255, 255)};

	const Scalar WHITE = Scalar(255, 255, 255);
	const Scalar GREEN = Scalar(200, 255, 100);
	const Scalar ORANGE = Scalar(100, 200, 255);

	VideoCapture capture;
	Point2d centerOfCapture;
	Point2d frameSize;
	double frameCount;

	void process(Mat &);
	Mat findRedColor(Mat &, Mat &);
	Mat findBlueColor(Mat &, Mat &);
	bool isRect(vector<Point> &, vector<Point> &);
	bool isCircle(vector<Point> &, vector<Point> &);

public:
	Operation(VideoCapture &);
	~Operation();

	void init();
	void mPutText(Mat &frame, vector<string> &);
};
