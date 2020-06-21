#include "FPS.h"
#include "Operation.h"

Operation::Operation(VideoCapture &capture) {
	char	key;
	double  beg;
	double  dur;
	bool	isPaused = false;

	FPS fps;
	Mat frame;

	for(int frameNo = 0; key = waitKey(1); frameNo += 1) {
		if(key == 27) {
			writeLogMessage(LOG_LEVEL_DEBUG, format("ESC pressed").c_str());
			break;
		}

		if(key == 32) {
			writeLogMessage(LOG_LEVEL_DEBUG, format("Space pressed").c_str());
			isPaused = !isPaused;
		}

		if(!isPaused) {
			beg = clock();
			if(!capture.read(frame)) {
				writeLogMessage(LOG_LEVEL_INFO, format("Frame could not be loaded").c_str());
				break;
			}
			process(frame);
			dur = clock() - beg;

			putText(frame, format("FPS : %.2lf", fps.calcAvgFps()),
					Point(10, 30), FONT_HERSHEY_SIMPLEX, .8, Scalar(0, 155, 255), 2);
			putText(frame, format("Frame Rate : %.2lf ms.", fps.calcAvgDur(dur)),
					Point(10, 60), FONT_HERSHEY_SIMPLEX, .8, Scalar(0, 155, 255), 2);
			putText(frame, format("Frame No : %d", frameNo),
					Point(10, 90), FONT_HERSHEY_SIMPLEX, .8, Scalar(0, 155, 255), 2);

			writeLogMessage(LOG_LEVEL_INFO, format("FPS : %4.2lf | Frame Rate : %4.2lf ms.",
												   fps.getAvarageFps(), fps.calcAvgDur(dur)).c_str());
		} else {
			putText(frame, format("Paused"), Point(10, 150), FONT_HERSHEY_SIMPLEX, .8, Scalar(0, 155, 255), 2);
		}
		imshow(WINDOW_NAME_MAIN, frame);
	}
}

void Operation::process(Mat &frame) {
	Mat pre = findColor(frame);
	imshow(WINDOW_NAME_PREP, pre);

	vector<vector<Point>> approx, contours;
	findContours(pre, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	approx.resize(contours.size());

	for(int i = 0; i < contours.size(); i++) {
		if(contourArea(contours[i]) > MIN_AREA) {
			findShape(frame, contours[i], approx[i]);
			drawContours(frame, approx, i, Scalar(0, 255, 255), 2);
		}
	}
	putText(frame, format("Number of Objects Detected : %d", approx.size()),
			Point(10, 120), FONT_HERSHEY_SIMPLEX, .8, Scalar(0, 155, 255), 2);
}

Mat Operation::findColor(Mat &frame) {
	Mat hsv, maskRedR, maskRedL, maskBlue, output;

	cvtColor(frame, hsv, COLOR_BGR2HSV);
	inRange(hsv, redR.lower, redR.upper, maskRedR);
	inRange(hsv, redL.lower, redL.upper, maskRedL);
	inRange(hsv, blue.lower, blue.upper, maskBlue);
	copyTo(frame, output, maskRedR | maskRedL | maskBlue);
	cvtColor(output, output, COLOR_BGR2GRAY);
	return output;
}

void Operation::findShape(Mat &frame, vector<Point> &contour, vector<Point> &approx) {
	double length = arcLength(contour, true);
	approxPolyDP(contour, approx, 0.02 * length, true);

	RotatedRect rect = minAreaRect(contour);
	putText(frame, format(". %s", approx.size() == 4 ? "Rectangle" : approx.size() > 6 ? "Circle" : "UGO"),
			Point2f(rect.center.x, rect.center.y), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 100, 255), 2);
}