#pragma once

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/utils/logger.hpp>

using namespace cv;
using namespace cv::utils::logging;
using namespace cv::utils::logging::internal;
using namespace std;

class Operation {
	public:
	Operation(VideoCapture &);
	~Operation();

	private:
	const float MIN_AREA = 1e3;
	const char *WINDOW_NAME_MAIN = "Window Main";
	const char *WINDOW_NAME_PREP = "Window Prep";

	const Scalar lowerBoundRed = Scalar(160, 93, 100);
	const Scalar upperBoundRed = Scalar(175, 200, 200);
	const Scalar lowerBoundBlue = Scalar(95, 160, 180);
	const Scalar upperBoundBlue = Scalar(110, 255, 255);

	VideoCapture capture;

	void process(Mat &);
	Mat findColor(Mat &);
	void findShape(Mat &, vector<Point> &, vector<Point> &);
};

