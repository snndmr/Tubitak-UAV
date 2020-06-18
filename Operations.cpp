#include "Operations.h"

Mat Operations::findColor(Mat image,
						  const Scalar &lowerBoundRed, const Scalar &upperBoundRed,
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

void Operations::findShape(Mat frame, vector<Point> &contour, vector<Point> &approx) {
	double length = arcLength(contour, true);
	approxPolyDP(contour, approx, 0.04 * length, true);

	RotatedRect rect = minAreaRect(contour);

	String text = "N/A";
	if(approx.size() == 4) {
		text = "Rectangle";
	} else if(approx.size() > 5) {
		text = "Circle";
	}
	putText(frame, text, Point2f(rect.center.x, rect.center.y), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 155, 255), 2);
	putText(frame, format("Type: %d", approx.size()), Point(10, 80), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 155, 255), 2);
}

bool Operations::capture(const char *camId) {
	VideoCapture capture(camId);

	if(!capture.isOpened()) {
		cout << format("\n %s could not be opened\n", camId);
		return EXIT_FAILURE;
	}

	int lastFps = 0;
	int fps = 0;

	char key;
	bool isPaused = false;

	Mat frame;
	vector<vector<Point>> approx, contours;

	clock_t beg = clock();
	do {
		key = waitKey(1);
		if(key == 32) { isPaused = !isPaused; }
		if(!isPaused) {
			capture.read(frame);
			if(frame.empty()) { break; }

			Mat pre = findColor(frame, lowerBoundRed, upperBoundRed, lowerBoundBlue, upperBoundBlue);
			imshow(WINDOW_NAME_PREP, pre);

			if(sum(frame)[0] > MIN_AREA) {
				findContours(pre, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
				approx.resize(contours.size());

				for(int i = 0; i < contours.size(); i++) {
					drawContours(frame, contours, i, Scalar(0, 255, 155), 3);
					findShape(frame, contours[i], approx[i]);
				}
				putText(frame, format("Fps: %d", lastFps), Point(10, 90), FONT_HERSHEY_SIMPLEX, 4, Scalar(0, 155, 255), 4);
			}
		} else {
			putText(frame, format("Paused"), Point(10, 40), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0, 155, 255), 2);
		}
		imshow(WINDOW_NAME_MAIN, frame);
		fps += 1;

		if(clock() - beg >= 1000) {
			beg = clock();
			lastFps = fps;
			fps = 0;
		}
	} while(key != 27);
	return EXIT_SUCCESS;
}