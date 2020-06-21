#pragma once

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utils/logger.hpp>

using namespace cv;
using namespace cv::utils::logging;
using namespace cv::utils::logging::internal;
using namespace std;

struct Bound {
	Scalar lower;
	Scalar upper;
};

class Operation {
	public:

	Operation(VideoCapture &);

	private:

	const float MIN_AREA = 1e3;

	const char *WINDOW_NAME_MAIN = "Window Main";
	const char *WINDOW_NAME_PREP = "Window Prep";

	const Bound redR = { Scalar(0, 100, 100), Scalar(10, 255, 255) };
	const Bound redL = { Scalar(160, 100, 100), Scalar(180, 255, 255) };
	const Bound blue = { Scalar(100, 150, 150) , Scalar(140, 255, 255) };

	const Scalar WHITE = Scalar(255, 255, 255);
	const Scalar GREEN = Scalar(200, 255, 100);
	const Scalar ORANGE = Scalar(100, 200, 255);

	VideoCapture capture;
	Point centerOfCapture;

	void process(Mat &);
	Mat findColor(Mat &);
	void findShape(Mat &, vector<Point> &, vector<Point> &);
};

