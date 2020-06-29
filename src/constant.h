#pragma once

#include "opencv2/core/types.hpp"

using namespace cv;

struct Bound {
    Scalar lower;
    Scalar upper;
};

const char *WINDOW_NAME_MAIN = "Window Main";
const char *WINDOW_NAME_PREP = "Window Prep";

const Scalar CUSTOM_WHITE = Scalar(255, 255, 255);
const Scalar CUSTOM_GREEN = Scalar(200, 255, 100);
const Scalar CUSTOM_ORANGE = Scalar(100, 200, 255);

const Bound BOUNDS_OF_BLUE = {Scalar(100, 100, 100), Scalar(130, 255, 255)};
const Bound BOUNDS_OF_RED_LEFT = {Scalar(0, 100, 100), Scalar(10, 255, 255)};
const Bound BOUNDS_OF_RED_RIGHT = {Scalar(160, 100, 100), Scalar(180, 255, 255)};

const int APPROX_SIZE_RECT = 4;
const int MIN_APPROX_SIZE_CIRCLE = 7;

const float MIN_AREA = 1e3;