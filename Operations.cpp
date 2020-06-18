#include "Operations.h"

Mat Operations::findColor(Mat image, const Scalar &lowerBound, const Scalar &upperBound) {
	Mat hsv, mask, output;

	cvtColor(image, hsv, COLOR_BGR2HSV);
	inRange(hsv, lowerBound, upperBound, mask);

	copyTo(image, output, mask);
	cvtColor(output, output, COLOR_BGR2GRAY);
	medianBlur(output, output, 3);
	return output;
}

void Operations::findShape(Mat frame, vector<Point> &contour, vector<Point> &approx) {
	double length = arcLength(contour, true);
	approxPolyDP(contour, approx, 0.04 * length, true);

	RotatedRect rect = minAreaRect(contour);
	putText(frame, format("%s", approx.size() == 4 ? "Rectangle" : "N/A"),
			Point2f(rect.center.x, rect.center.y), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 155, 255), 2);
}

bool Operations::capture(const char *camId) {
	VideoCapture capture(camId);

	if(!capture.isOpened()) {
		cout << format("\n %s could not be opened\n", camId);
		return EXIT_FAILURE;
	}

	char key;
	bool isPaused = false;

	int64 beg;
	Mat frame;
	vector<vector<Point>> approx, contours;

	while(true) {
		key = waitKey(1);

		if(key == 27) { break; }
		if(key == 32) { isPaused = !isPaused; }

		if(!isPaused) {
			beg = getTickCount();
			capture.read(frame);
			if(frame.empty()) { break; }

			Mat pre = findColor(frame, lowerBoundBlue, upperBoundBlue);
			imshow(WINDOW_NAME_PREP, pre);

			if(sum(frame)[0] > MIN_AREA) {
				findContours(pre, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
				approx.resize(contours.size());

				for(int i = 0; i < contours.size(); i++) {
					drawContours(frame, contours, i, Scalar(0, 255, 155), 3);
					findShape(frame, contours[i], approx[i]);
				}
			}
			putText(frame, format("Fps: %.2lf | Objects: %d", getTickFrequency() / (getTickCount() - beg), approx.size()),
					Point(10, 40), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 155, 255), 2);
		} else {
			putText(frame, format("Paused"), Point(10, 80), FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 155, 255), 2);
		}
		imshow(WINDOW_NAME_MAIN, frame);
	}
	return EXIT_SUCCESS;
}