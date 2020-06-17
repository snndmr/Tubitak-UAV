#include "Operations.h"

Mat Operations::findColor(Mat image) {
	Mat hsv, mask, output;

	cvtColor(image, hsv, COLOR_BGR2HSV);
	inRange(hsv, lowerBoundBlue, upperBoundBlue, mask);

	copyTo(image, output, mask);
	cvtColor(output, output, COLOR_BGR2GRAY);
	medianBlur(output, output, 3);
	return output;
}

void Operations::findShape(Mat frame, vector<Point> &contour, vector<Point> &approx) {
	double length = arcLength(contour, true);
	approxPolyDP(contour, approx, 0.04 * length, true);

	RotatedRect rect = minAreaRect(contour);
	putText(frame, format(". %s", approx.size() == 4 ? "Rect" : "N/A"),
			Point2f(rect.center.x, rect.center.y), FONT_HERSHEY_DUPLEX, 1.8, Scalar(0, 155, 255), 3);
}

bool Operations::capture(const char *camId) {
	VideoCapture capture(camId);

	if(!capture.isOpened()) {
		cout << format("\n %s could not be opened\n", camId);
		return EXIT_FAILURE;
	}

	char key;
	bool isPaused = false;

	Mat frame;
	vector<vector<Point>> approx;
	vector<vector<Point>> contours;

	do {
		key = waitKey(1);

		if(key == ' ') { isPaused = !isPaused; }
		if(isPaused) { continue; }

		capture.read(frame);
		if(frame.empty()) { break; }

		Mat pre = findColor(frame);

		if(sum(frame)[0] > MIN_AREA) {
			findContours(pre, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
			approx.resize(contours.size());

			for(int i = 0; i < contours.size(); i++) {
				drawContours(frame, contours, i, Scalar(0, 255, 155), 3);
				findShape(frame, contours[i], approx[i]);
			}
		}
		imshow(WINDOW_NAME_PREP, pre);
		imshow(WINDOW_NAME_MAIN, frame);
	} while(key != 27);
	return EXIT_SUCCESS;
}
