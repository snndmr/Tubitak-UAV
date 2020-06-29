#include "operation.h"

#include "constant.h"
#include "fps.h"

Operation::Operation(VideoCapture &capture) : capture(capture) {
    setFrameSize(Point2d(capture.get(CAP_PROP_FRAME_WIDTH), capture.get(CAP_PROP_FRAME_HEIGHT)));
    setCenterOfCapture(Point2d(getFrameSize().x / 2, getFrameSize().y / 2));
    setFrameCount(capture.get(CAP_PROP_FRAME_COUNT));
    init();
}

Operation::~Operation() {
    cout << "\nCapture is finished.\n";
    capture.release();
    destroyAllWindows();
}

void Operation::init() {
    double beg = 0.0;
    double dur = 0.0;
    bool isPaused = false;
    char pressedKey = '\0';

    Mat frame;
    FPS *fps = new FPS();
    vector<string> texts;

    for (unsigned int frameNo = 0; pressedKey = waitKey(1);) {
        if (pressedKey == 27) {
            cout << "\n(27) ESC pressed";
            delete fps;
            break;
        }

        if (pressedKey == 32) {
            cout << "\n(32) Space pressed";
            isPaused = !isPaused;
        }

        if (!isPaused) {
            beg = fps->mClock();
            if (!capture.read(frame)) {
                cout << "\nFrame could not be loaded";
                break;
            }

            process(frame);
            dur = fps->mClock() - beg;

            drawMarker(frame, getCenterOfCapture(), CUSTOM_WHITE, MARKER_CROSS, 30, 2);

            mPutText(frame, format("FPS : %.2lf", fps->calcAvgFps()), Point(5, 30), CUSTOM_ORANGE);
            mPutText(frame, format("Frame Rate : %.2lf ms.", fps->calcAvgDur(dur)), Point(5, 60), CUSTOM_ORANGE);
            mPutText(frame, format("Frame No : %d out of %.0lf", frameNo, getFrameCount()), Point(5, 90), CUSTOM_ORANGE);
            mPutText(frame, format("Frame Size : %.1lf x %.1lf", getFrameSize().x, getFrameSize().y), Point(5, 120), CUSTOM_ORANGE);

            frameNo += 1;
        } else {
            mPutText(frame, format("Paused"), Point(5, 150), CUSTOM_ORANGE);
        }
        imshow(WINDOW_NAME_MAIN, frame);
    }
}

void Operation::process(Mat &frame) {
    Mat hsv;
    cvtColor(frame, hsv, COLOR_BGR2HSV);
    medianBlur(hsv, hsv, 5);

    vector<vector<Point>> contoursRed, contursBlue;
    findContours(findRedColor(frame, hsv), contoursRed, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    findContours(findBlueColor(frame, hsv), contursBlue, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    vector<vector<Point>> approxRed(contoursRed.size()), approxBlue(contursBlue.size());

    for (int i = 0; i < contoursRed.size(); i++) {
        if (contourArea(contoursRed[i]) > MIN_AREA && isCircle(contoursRed[i], approxRed[i])) {
            drawContours(frame, approxRed, i, CUSTOM_ORANGE, 2);
            RotatedRect rect = minAreaRect(contoursRed[i]);
            line(frame, centerOfCapture, rect.center, CUSTOM_GREEN, 2);
            mPutText(frame, format(" Red Circle (%.1f, %.1f)", rect.center.x, rect.center.y), rect.center, CUSTOM_GREEN);
        }
    }

    for (int i = 0; i < contursBlue.size(); i++) {
        if (contourArea(contursBlue[i]) > MIN_AREA && isRectangle(contursBlue[i], approxBlue[i])) {
            drawContours(frame, approxBlue, i, CUSTOM_ORANGE, 2);
            RotatedRect rect = minAreaRect(contursBlue[i]);
            line(frame, centerOfCapture, rect.center, CUSTOM_GREEN, 2);
            mPutText(frame, format(" %s (%.1f, %.1f)", isSquare(approxBlue[i]) ? "Square" : "Rectangle", rect.center.x, rect.center.y), rect.center, CUSTOM_GREEN);
        }
    }
}

void Operation::mPutText(Mat &frame, string text, Point point, Scalar color) {
    putText(frame, text, point, FONT_HERSHEY_SIMPLEX, .8, color, 2);
}

Mat Operation::findRedColor(Mat &frame, Mat &hsv) {
    Mat maskRedRight, maskRedLeft, output;
    inRange(hsv, BOUNDS_OF_RED_RIGHT.lower, BOUNDS_OF_RED_RIGHT.upper, maskRedRight);
    inRange(hsv, BOUNDS_OF_RED_LEFT.lower, BOUNDS_OF_RED_LEFT.upper, maskRedLeft);
    bitwise_and(frame, frame, output, maskRedRight | maskRedLeft);
    cvtColor(output, output, COLOR_BGR2GRAY);
    return output;
}

Mat Operation::findBlueColor(Mat &frame, Mat &hsv) {
    Mat maskBlue, output;
    inRange(hsv, BOUNDS_OF_BLUE.lower, BOUNDS_OF_BLUE.upper, maskBlue);
    bitwise_and(frame, frame, output, maskBlue);
    cvtColor(output, output, COLOR_BGR2GRAY);
    return output;
}

bool Operation::isSquare(vector<Point> &approx) {
    RotatedRect rect = minAreaRect(approx);
    float aspectRatio = rect.size.width / rect.size.height;
    return 0.9 < aspectRatio && aspectRatio < 1.1 ? true : false;
}

bool Operation::isRectangle(vector<Point> &contour, vector<Point> &approx) {
    approxPolyDP(contour, approx, 0.01 * arcLength(contour, true), true);
    return approx.size() == APPROX_SIZE_RECT && isContourConvex(approx) ? true : false;
}

bool Operation::isCircle(vector<Point> &contour, vector<Point> &approx) {
    approxPolyDP(contour, approx, 0.02 * arcLength(contour, true), true);
    return approx.size() > MIN_APPROX_SIZE_CIRCLE && isContourConvex(approx) ? true : false;
}
