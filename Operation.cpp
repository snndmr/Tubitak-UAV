#include "FPS.h"
#include "Operation.h"

Operation::Operation(VideoCapture &capture) : capture(capture) {
	char	key = NULL;
	bool	isPaused = false;
	double  beg = 0.0;
	double  dur = 0.0;

	FPS fps;
	Mat frame;

	for(int frameNo = 0; key = waitKey(1); frameNo += 1) {
		if(key == 27) { break; }
		if(key == 32) { isPaused = !isPaused; }

		if(!isPaused) {
			beg = clock();
			if(!capture.read(frame)) { break; }
			process(frame);
			dur = clock() - beg;

			putText(frame, format("FPS : %.1lf", fps.averagefps()), Point(10, 30), FONT_HERSHEY_SIMPLEX, .8, Scalar(0, 155, 255), 2);
			putText(frame, format("Frame Rate : %.2lf ms.", fps.averagedur(dur)), Point(10, 60), FONT_HERSHEY_SIMPLEX, .8, Scalar(0, 155, 255), 2);
			putText(frame, format("Frame No : %d", frameNo), Point(10, 90), FONT_HERSHEY_SIMPLEX, .8, Scalar(0, 155, 255), 2);
		} else {
			putText(frame, format("Paused"), Point2f(10, 150), FONT_HERSHEY_SIMPLEX, .8, Scalar(0, 155, 255), 2);
		}
		imshow(WINDOW_NAME_MAIN, frame);
	}
}

void Operation::process(Mat &frame) {
	Mat pre = findColor(frame);
	imshow("Preprocess", pre);

	vector<vector<Point>> approx, contours;
	findContours(pre, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	approx.resize(contours.size());

	for(int i = 0; i < contours.size(); i++) {
		if(contourArea(contours[i]) > MIN_AREA) {
			findShape(frame, contours[i], approx[i]);
			drawContours(frame, approx, i, Scalar(0, 255, 255), 2);
		}
	}
	putText(frame, format("Number of Objects Detected : %d", approx.size()), Point(10, 120), FONT_HERSHEY_SIMPLEX, .8, Scalar(0, 155, 255), 2);
}

Mat Operation::findColor(Mat &frame) {
	Mat hsv, maskRed, maskBlue, output;

	cvtColor(frame, hsv, COLOR_BGR2HSV);
	inRange(hsv, lowerBoundRed, upperBoundRed, maskRed);
	inRange(hsv, lowerBoundBlue, upperBoundBlue, maskBlue);
	copyTo(frame, output, maskRed | maskBlue);
	cvtColor(output, output, COLOR_BGR2GRAY);
	return output;
}

void Operation::findShape(Mat &frame, vector<Point> &contour, vector<Point> &approx) {
	double length = arcLength(contour, true);
	approxPolyDP(contour, approx, 0.02 * length, true);

	RotatedRect rect = minAreaRect(contour);
	putText(frame, format(". %s", approx.size() == 4 ? "Rectangle" : approx.size() > 7 ? "Circle" : "UGO"),
			Point2f(rect.center.x, rect.center.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 100, 255), 2);
}

Operation::~Operation() {
	if(capture.isOpened()) {
		capture.release();
	}
}