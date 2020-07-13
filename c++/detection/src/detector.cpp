#include "detector.hpp"

Detector::Detector() {}

Detector::~Detector() {}

vector<Bundle> Detector::process(Mat image) {
    Mat hsv;
    cvtColor(image, hsv, COLOR_BGR2HSV);
    medianBlur(hsv, hsv, 5);

    // imshow("Masked Red", findRedColor(image, hsv));
    // imshow("Masked Blue", findBlueColor(image, hsv));

    vector<Bundle> bundles;
    vector<vector<Point>> contoursRed, contoursBlue;
    findContours(findRedColor(image, hsv), contoursRed, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    findContours(findBlueColor(image, hsv), contoursBlue, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    vector<vector<Point>> approxRed(contoursRed.size()), approxBlue(contoursBlue.size());

    for (int i = 0; i < contoursRed.size(); i++) {
        if (contourArea(contoursRed[i]) > MIN_AREA && isCircle(contoursRed[i], approxRed[i])) {
            RotatedRect rect = minAreaRect(approxRed[i]);
            bundles.push_back({CIRCLE, rect.center, approxRed[i]});
        }
    }

    for (int i = 0; i < contoursBlue.size(); i++) {
        if (contourArea(contoursBlue[i]) > MIN_AREA && isRectangle(contoursBlue[i], approxBlue[i])) {
            RotatedRect rect = minAreaRect(approxBlue[i]);
            bundles.push_back({isSquare(approxBlue[i]) ? SQUARE : RECT, rect.center, approxBlue[i]});
        }
    }

    return bundles;
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

bool Detector::isCircle(vector<Point> &contour, vector<Point> &approx) {
    approxPolyDP(contour, approx, 0.02 * arcLength(contour, true), true);
    return approx.size() > MIN_APPROX_SIZE_CIRCLE && isContourConvex(approx) ? true : false;
}

bool Detector::isRectangle(vector<Point> &contour, vector<Point> &approx) {
    approxPolyDP(contour, approx, 0.01 * arcLength(contour, true), true);
    return approx.size() == APPROX_SIZE_RECT && isContourConvex(approx) ? true : false;
}