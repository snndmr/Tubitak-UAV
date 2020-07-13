#include "detector.hpp"

Detector::Detector() {}

Detector::~Detector() {}

Bundle Detector::process(Mat frame, Point2d pos) {
    Mat hsv;
    cvtColor(frame, hsv, COLOR_BGR2HSV);
    medianBlur(hsv, hsv, 3);

    Mat maskedRed  = findRedColor(frame, hsv);
    Mat maskedBlue = findBlueColor(frame, hsv);

    // imshow("red", maskedRed);
    // imshow("blue", maskedBlue);

    vector<vector<Point>> contoursRed, contursBlue;
    findContours(maskedRed, contoursRed, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    findContours(maskedBlue, contursBlue, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    vector<vector<Point>> approxRed(contoursRed.size()), approxBlue(contursBlue.size());

    Point center(frame.cols / 2, frame.rows / 2);

    for (int i = 0; i < contoursRed.size(); i++) {
        if (contourArea(contoursRed[i]) > MIN_AREA && isCircle(contoursRed[i], approxRed[i])) {
            drawContours(frame, approxRed, i, CUSTOM_ORANGE, 2);
            RotatedRect rect = minAreaRect(contoursRed[i]);
            line(frame, center, rect.center, CUSTOM_GREEN, 2);
            mPutText(frame, format(" Red Circle (%.1f, %.1f)", rect.center.x, rect.center.y), rect.center, CUSTOM_GREEN);
            redCircleCounter += 1;

            if (redCircleCounter > 100) {
                redCircleCounter = 0;
                return Bundle{CIRCLE, rect.center};
            }
        } else {
            redCircleCounter = 0;
        }
    }

    for (int i = 0; i < contursBlue.size(); i++) {
        if (contourArea(contursBlue[i]) > MIN_AREA && isRectangle(contursBlue[i], approxBlue[i])) {
            drawContours(frame, approxBlue, i, CUSTOM_ORANGE, 2);
            RotatedRect rect = minAreaRect(contursBlue[i]);
            line(frame, center, rect.center, CUSTOM_GREEN, 2);

            if (isSquare(approxBlue[i])) {
                mPutText(frame, format(" Square (%.1f, %.1f)", rect.center.x, rect.center.y), rect.center, CUSTOM_GREEN);
                blueSquareCounter += 1;

                if (blueSquareCounter > 100) {
                    blueSquareCounter = 0;
                    return Bundle{SQUARE, rect.center};
                }

            } else {
                mPutText(frame, format(" Rectangle (%.1f, %.1f)", rect.center.x, rect.center.y), rect.center, CUSTOM_GREEN);
                blueSquareCounter = 0;
            }
        } else {
            blueSquareCounter = 0;
        }
    }

    mPutText(frame, format("Position of Drone: %.1lf, %.1lf", pos.x, pos.y), Point(10, 30), CUSTOM_ORANGE);
    mPutText(frame, format("RC: %03d-100", redCircleCounter), Point(10, 50), CUSTOM_ORANGE);
    mPutText(frame, format("BS: %03d-100", blueSquareCounter), Point(10, 70), CUSTOM_ORANGE);
    drawMarker(frame, center, Scalar(255, 255, 255), MARKER_CROSS, 20, 2);

    return Bundle{UNKNOWN, Point2d(0, 0)};
}

void Detector::mPutText(Mat frame, string text, Point point, Scalar color) {
    putText(frame, text, point, FONT_HERSHEY_SIMPLEX, .6, color, 2);
}

Mat Detector::findRedColor(Mat frame, Mat hsv) {
    Mat maskRedRight, maskRedLeft, output;
    inRange(hsv, BOUNDS_OF_RED_RIGHT.lower, BOUNDS_OF_RED_RIGHT.upper, maskRedRight);
    inRange(hsv, BOUNDS_OF_RED_LEFT.lower, BOUNDS_OF_RED_LEFT.upper, maskRedLeft);
    bitwise_and(frame, frame, output, maskRedRight | maskRedLeft);
    cvtColor(output, output, COLOR_BGR2GRAY);
    return output;
}

Mat Detector::findBlueColor(Mat frame, Mat hsv) {
    Mat maskBlue, output;
    inRange(hsv, BOUNDS_OF_BLUE.lower, BOUNDS_OF_BLUE.upper, maskBlue);
    bitwise_and(frame, frame, output, maskBlue);
    cvtColor(output, output, COLOR_BGR2GRAY);
    return output;
}

bool Detector::isSquare(vector<Point> &approx) {
    RotatedRect rect  = minAreaRect(approx);
    float aspectRatio = rect.size.width / rect.size.height;
    return 0.9 < aspectRatio && aspectRatio < 1.1 ? true : false;
}

bool Detector::isRectangle(vector<Point> &contour, vector<Point> &approx) {
    approxPolyDP(contour, approx, 0.01 * arcLength(contour, true), true);
    return approx.size() == APPROX_SIZE_RECT && isContourConvex(approx) ? true : false;
}

bool Detector::isCircle(vector<Point> &contour, vector<Point> &approx) {
    approxPolyDP(contour, approx, 0.02 * arcLength(contour, true), true);
    return approx.size() > MIN_APPROX_SIZE_CIRCLE && isContourConvex(approx) ? true : false;
}