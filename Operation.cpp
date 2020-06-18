#include "FPS.h"
#include "Operation.h"

Operation::Operation(const char *camId) {
	if(capture.open(camId)) {
		FPS fps;
		char key = 0;
		bool isPaused = false;

		Mat frame;
		while(true) {
			key = waitKey(1);
			if(key == 27) { break; }
			if(key == 32) { isPaused = !isPaused; }

			if(!isPaused) {
				double start = clock();
				capture.read(frame);
				if(frame.empty()) { break; }
				process(frame);
				double dur = clock() - start;

				putText(frame, format("FPS: %.2lf | Avarage Time Per Frame: %.2lf ms.", fps.averagefps(), fps.averagedur(dur)),
						Point2f(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 255), 2);
			} else {
				putText(frame, format("Paused"), Point2f(10, 90), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 255), 2);
			}
			imshow(WINDOW_NAME_MAIN, frame);
		}
	}
}

Operation::~Operation() {
	if(capture.isOpened()) {
		capture.release();
	}
}

void Operation::process(Mat &frame) {
	vector<vector<Point>> approx, contours;

	Mat pre = findColor(frame, lowerBoundRed, upperBoundRed, lowerBoundBlue, upperBoundBlue);
	imshow(WINDOW_NAME_PREP, pre);

	findContours(pre, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	approx.resize(contours.size());
	for(int i = 0; i < contours.size(); i++) {
		if(contourArea(contours[i]) > MIN_AREA) {
			drawContours(frame, contours, i, Scalar(0, 255, 100), 2);
			findShape(frame, contours[i], approx[i]);
		}
	}
	putText(frame, format("Number of Objects Detected: %d", approx.size()),
			Point2f(10, 60), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 255), 2);
}

Mat Operation::findColor(Mat image, const Scalar &lowerBoundRed, const Scalar &upperBoundRed,
						 const Scalar &lowerBoundBlue, const Scalar &upperBoundBlue) {
	Mat hsv, maskRed, maskBlue, output;

	cvtColor(image, hsv, COLOR_BGR2HSV);
	inRange(hsv, lowerBoundRed, upperBoundRed, maskRed);
	inRange(hsv, lowerBoundBlue, upperBoundBlue, maskBlue);

	copyTo(image, output, maskRed | maskBlue);
	cvtColor(output, output, COLOR_BGR2GRAY);
	medianBlur(output, output, 3);
	return output;
}

void Operation::findShape(Mat frame, vector<Point> &contour, vector<Point> &approx) {
	double length = arcLength(contour, true);
	approxPolyDP(contour, approx, 0.04 * length, true);

	RotatedRect rect = minAreaRect(contour);
	putText(frame, format(". %s", approx.size() == 4 ? "Rectangle" : approx.size() > 5 ? "Circle" : "UGO"),
			Point2f(rect.center.x, rect.center.y), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 100, 255), 2);
}