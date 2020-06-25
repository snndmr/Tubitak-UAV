#include "FPS.h"
#include "Operation.h"

Operation::Operation(VideoCapture &capture) : capture(capture)
{
	cout << "\n Capture was started.";
	frameSize = Point2d(capture.get(CAP_PROP_FRAME_WIDTH), capture.get(CAP_PROP_FRAME_HEIGHT));
	centerOfCapture = Point2d(frameSize.x / 2, frameSize.y / 2);
	frameCount = capture.get(CAP_PROP_FRAME_COUNT);
	init();
}

Operation::~Operation()
{
	cout << "\n Capture is finished.";
	capture.release();
	destroyAllWindows();
}

void Operation::init()
{
	double beg = 0.0;
	double dur = 0.0;
	bool isPaused = false;
	char pressedKey = '\0';

	Mat frame;
	FPS *fps = new FPS();
	vector<string> texts;

	for (unsigned int frameNo = 0; pressedKey = waitKey(1);)
	{
		if (pressedKey == 27)
		{
			cout << "\n (27) ESC pressed";
			delete fps;
			break;
		}

		if (pressedKey == 32)
		{
			cout << "\n (32) Space pressed";
			isPaused = !isPaused;
		}

		if (!isPaused)
		{
			beg = fps->mClock();
			if (!capture.read(frame))
			{
				cout << "\n Frame could not be loaded";
				break;
			}

			process(frame);
			dur = fps->mClock() - beg;
			texts = {
				format("FPS : %.2lf", fps->calcAvgFps()),
				format("Frame Rate : %.2lf ms.", fps->calcAvgDur(dur)),
				format("Frame No : %d out of %.0lf", frameNo, frameCount),
				format("Frame Size : %.1lf x %.1lf", frameSize.x, frameSize.y)};
			drawMarker(frame, centerOfCapture, WHITE, MARKER_CROSS, 20, 2);
			frameNo += 1;
		}
		else
		{
			if (texts.size() == 4)
			{
				texts.push_back("Paused");
			}
		}
		mPutText(frame, texts);
		imshow(WINDOW_NAME_MAIN, frame);
	}
}

void Operation::process(Mat &frame)
{
	Mat hsv;
	medianBlur(frame, frame, 3);
	cvtColor(frame, hsv, COLOR_BGR2HSV);

	vector<vector<Point>> contoursRed, contursBlue;
	findContours(findRedColor(frame, hsv), contoursRed, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	findContours(findBlueColor(frame, hsv), contursBlue, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<vector<Point>> approxRed(contoursRed.size()), approxBlue(contursBlue.size());

	for (int i = 0; i < contoursRed.size(); i++)
	{
		if (contourArea(contoursRed[i]) > MIN_AREA && isCircle(contoursRed[i], approxRed[i]))
		{
			drawContours(frame, approxRed, i, ORANGE, 2);

			RotatedRect rect = minAreaRect(contoursRed[i]);
			line(frame, centerOfCapture, rect.center, GREEN, 2);
			putText(frame, format(" Red Circle (%.1f, %.1f)", rect.center.x, rect.center.y), rect.center, FONT_HERSHEY_SIMPLEX, 0.8, GREEN, 2);
		}
	}

	for (int i = 0; i < contursBlue.size(); i++)
	{
		if (contourArea(contursBlue[i]) > MIN_AREA && isRect(contursBlue[i], approxBlue[i]))
		{
			drawContours(frame, approxBlue, i, ORANGE, 2);

			RotatedRect rect = minAreaRect(contursBlue[i]);
			line(frame, centerOfCapture, rect.center, GREEN, 2);
			putText(frame, format(" Blue Rectangle (%.1f, %.1f)", rect.center.x, rect.center.y), rect.center, FONT_HERSHEY_SIMPLEX, 0.8, GREEN, 2);
		}
	}
}

void Operation::mPutText(Mat &frame, vector<string> &texts)
{
	for (unsigned int i = 0; i < texts.size(); i++)
	{
		putText(frame, texts[i], Point(5, (i + 1) * 30), FONT_HERSHEY_SIMPLEX, .8, ORANGE, 2);
	}
}

Mat Operation::findRedColor(Mat &frame, Mat &hsv)
{
	Mat maskRedR, maskRedL, output;
	inRange(hsv, redR.lower, redR.upper, maskRedR);
	inRange(hsv, redL.lower, redL.upper, maskRedL);
	bitwise_and(frame, frame, output, maskRedR | maskRedL);
	cvtColor(output, output, COLOR_BGR2GRAY);
	return output;
}

Mat Operation::findBlueColor(Mat &frame, Mat &hsv)
{
	Mat maskBlue, output;
	inRange(hsv, blue.lower, blue.upper, maskBlue);
	bitwise_and(frame, frame, output, maskBlue);
	cvtColor(output, output, COLOR_BGR2GRAY);
	return output;
}

bool Operation::isRect(vector<Point> &contour, vector<Point> &approx)
{
	approxPolyDP(contour, approx, 0.01 * arcLength(contour, true), true);
	return approx.size() == 4 && isContourConvex(approx) ? true : false;
}

bool Operation::isCircle(vector<Point> &contour, vector<Point> &approx)
{
	approxPolyDP(contour, approx, 0.02 * arcLength(contour, true), true);
	return approx.size() > 6 && isContourConvex(approx) ? true : false;
}
