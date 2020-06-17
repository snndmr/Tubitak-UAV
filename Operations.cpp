#include "Operations.h"

Operations::Operations() {}

Operations::~Operations() {}

bool Operations::capture(const char *camId) {
	VideoCapture capture(camId);

	if(!capture.isOpened()) {
		cout << format("\n %s could not be opened\n", camId);
		return EXIT_FAILURE;
	}

	char pressedKey;
	bool isPaused = false;

	vector<vector<Point>> approx;
	vector<vector<Point>> contours;

	Mat frame;
	while(true) {
		pressedKey = waitKey(1);

		if(pressedKey == ' ') {
			isPaused = !isPaused;
		}

		if(isPaused) {
			continue;
		}

		capture.read(frame);
		if(frame.empty()) break;

		Mat pre = findColor(frame);

		float area = (float) sum(frame)[0];
		if(area > 1e5) {
			findContours(pre, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
			approx.resize(contours.size());

			for(int i = 0; i < contours.size(); i++) {
				drawContours(frame, contours, i, Scalar(0, 255, 255), 5);

				double length = arcLength(contours[i], true);
				approxPolyDP(contours[i], approx[i], 0.04 * length, true);

				RotatedRect rect = minAreaRect(contours[i]);
				putText(frame, format("%s", approx[i].size() == 4 ? "Rect" : "Undefined"),
						Point2f(rect.center.x, rect.center.y), FONT_HERSHEY_DUPLEX, 2, Scalar(0, 0, 255), 2);
			}
		}
		imshow(WINDOW_NAME_MAIN, frame);
	}
	return EXIT_SUCCESS;
}

Mat Operations::findColor(Mat image) {
	Mat hsv, mask, output;

	cvtColor(image, hsv, COLOR_BGR2HSV);
	inRange(hsv, lowerBoundBlue, upperBoundBlue, mask);

	copyTo(image, output, mask);
	cvtColor(output, output, COLOR_BGR2GRAY);
	medianBlur(output, output, 3);
	return output;
}
