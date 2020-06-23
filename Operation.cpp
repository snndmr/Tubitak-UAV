#include "FPS.h"
#include "Operation.h"

Operation::Operation(VideoCapture &capture) {
	centerOfCapture = Point2d(capture.get(CAP_PROP_FRAME_WIDTH) / 2,
							  capture.get(CAP_PROP_FRAME_HEIGHT) / 2);
	char key;
	double beg;
	double dur;
	bool isPaused = false;

	FPS fps;
	Mat frame;

	for(int frameNo = 0; key = waitKey(1); frameNo += 1) {
		if(key == 27) {
			cout << format("\n ESC pressed");
			break;
		}

		if(key == 32) {
			cout << format("\n Space pressed");
			isPaused = !isPaused;
		}

		if(!isPaused) {
			beg = clock();
			if(!capture.read(frame)) {
				cout << format("\n Frame could not be loaded");
				break;
			}
			process(frame);
			dur = clock() - beg;

			putText(frame, format("FPS : %.2lf", fps.calcAvgFps()),
					Point(10, 30), FONT_HERSHEY_SIMPLEX, .8, ORANGE, 2);
			putText(frame, format("Frame Rate : %.2lf ms.", fps.calcAvgDur(dur)),
					Point(10, 60), FONT_HERSHEY_SIMPLEX, .8, ORANGE, 2);
			putText(frame, format("Frame No : %d", frameNo),
					Point(10, 90), FONT_HERSHEY_SIMPLEX, .8, ORANGE, 2);

			drawMarker(frame, centerOfCapture, WHITE, MARKER_CROSS, 20, 2);
		} else {
			putText(frame, format("Paused"),
					Point(10, 120), FONT_HERSHEY_SIMPLEX, .8, ORANGE, 2);
		}
		imshow(WINDOW_NAME_MAIN, frame);
	}
}

void Operation::process(Mat &frame) {
	Mat hsv;
	cvtColor(frame, hsv, COLOR_BGR2HSV);

	vector<vector<Point>> contoursRed, contursBlue;
	findContours(findRedColor(frame, hsv), contoursRed, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	findContours(findBlueColor(frame, hsv), contursBlue, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<vector<Point>> approxRed(contoursRed.size()), approxBlue(contursBlue.size());

	for(int i = 0; i < contoursRed.size(); i++) {
		if(contourArea(contoursRed[i]) > MIN_AREA && isCircle(contoursRed[i], approxRed[i])) {
			drawContours(frame, approxRed, i, ORANGE, 2);

			RotatedRect rect = minAreaRect(contoursRed[i]);
			line(frame, centerOfCapture, rect.center, GREEN, 2);
			putText(frame, format("Circle (%.1f, %.1f)",
								  rect.center.x, rect.center.y), rect.center, FONT_HERSHEY_SIMPLEX, 0.8, GREEN, 2);
		}
	}

	for(int i = 0; i < contursBlue.size(); i++) {
		if(contourArea(contursBlue[i]) > MIN_AREA && isRect(contursBlue[i], approxBlue[i])) {
			drawContours(frame, approxBlue, i, ORANGE, 2);

			RotatedRect rect = minAreaRect(contursBlue[i]);
			line(frame, centerOfCapture, rect.center, GREEN, 2);
			putText(frame, format("Rectangle (%.1f, %.1f)",
								  rect.center.x, rect.center.y), rect.center, FONT_HERSHEY_SIMPLEX, 0.8, GREEN, 2);
		}
	}
}

Mat Operation::findRedColor(Mat &frame, Mat &hsv) {
	Mat maskRedR, maskRedL, output;
	inRange(hsv, redR.lower, redR.upper, maskRedR);
	inRange(hsv, redL.lower, redL.upper, maskRedL);
	bitwise_and(frame, frame, output, maskRedR | maskRedL);
	cvtColor(output, output, COLOR_BGR2GRAY);
	return output;
}

Mat Operation::findBlueColor(Mat &frame, Mat &hsv) {
	Mat maskBlue, output;
	inRange(hsv, blue.lower, blue.upper, maskBlue);
	bitwise_and(frame, frame, output, maskBlue);
	cvtColor(output, output, COLOR_BGR2GRAY);
	return output;
}

bool Operation::isRect(vector<Point> &contour, vector<Point> &approx) {
	approxPolyDP(contour, approx, 0.04 * arcLength(contour, true), true);
	return approx.size() == 4 ? true : false;
}

bool Operation::isCircle(vector<Point> &contour, vector<Point> &approx) {
	approxPolyDP(contour, approx, 0.01 * arcLength(contour, true), true);
	return approx.size() > 6 ? true : false;
}