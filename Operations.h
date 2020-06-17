#pragma once

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

class Operations {
	public:
	Operations();
	~Operations();
	bool capture(const char *);

	private:
	const char *WINDOW_NAME_MAIN = "Window Main";
	const char *WINDOW_NAME_PREP = "Window Prep";

	const Scalar lowerBoundBlue = Scalar(80, 150, 150);
	const Scalar upperBoundBlue = Scalar(120, 255, 255);

	Mat findColor(Mat);
};

