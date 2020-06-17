#pragma once

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

class Operations {
	public:
	bool capture(const char *);

	private:
	const float MIN_AREA = 1e5;

	const char *WINDOW_NAME_MAIN = "Window Main";
	const char *WINDOW_NAME_PREP = "Window Prep";

	const Scalar lowerBoundBlue = Scalar( 90, 155, 155);
	const Scalar upperBoundBlue = Scalar(130, 255, 255);

	Mat findColor(Mat);
	void findShape(Mat, vector<Point> &, vector<Point> &);
};

