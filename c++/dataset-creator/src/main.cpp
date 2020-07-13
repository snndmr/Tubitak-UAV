#include <time.h>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;

const int COUNT_OF_OBJECT    = 10000;
const char *WINDOW_NAME_MAIN = "Window";

const Size SIZE_OF_IMAGE = Size(512, 512);
const Scalar BLACK_8UC1  = Scalar(0);
const Scalar WHITE_8UC1  = Scalar(255);

void drawRotatedRectangle(Mat &image, Point centerPoint, Size rectangleSize, double rotationDegrees) {
    RotatedRect rotatedRectangle(centerPoint, rectangleSize, rotationDegrees);

    Point2f vertices2f[4];
    rotatedRectangle.points(vertices2f);

    Point vertices[4];
    for (int i = 0; i < 4; ++i) vertices[i] = vertices2f[i];
    fillConvexPoly(image, vertices, 4, WHITE_8UC1);
}

int main(int argc, char const **argv) {
    srand(time(0));

    Mat instant, image(SIZE_OF_IMAGE, CV_8UC1, BLACK_8UC1);

    int radius         = 0;
    int edgeLength     = 0;
    int rotationDegree = 0;
    Point center       = Point(SIZE_OF_IMAGE.width / 2, SIZE_OF_IMAGE.height / 2);

    for (unsigned int index = 1; index <= COUNT_OF_OBJECT; index++) {
        image.copyTo(instant);

        radius = rand() % min(SIZE_OF_IMAGE.width, SIZE_OF_IMAGE.height) / 2;
        circle(instant, center, radius, WHITE_8UC1, FILLED, LINE_4);
        imwrite(format("images/circle/red_circle_%d.jpg", index), instant);

        image.copyTo(instant);
        rotationDegree = rand() % 360;
        edgeLength     = rand() % min(SIZE_OF_IMAGE.width, SIZE_OF_IMAGE.height) / sqrt(2);
        drawRotatedRectangle(instant, center, Size(edgeLength, edgeLength), rotationDegree);
        imwrite(format("images/square/blue_square_%d.jpg", index), instant);

        printf("\r %d-%d (%%%.2lf) was processed", index, COUNT_OF_OBJECT, 100 * (index * 1.0 / COUNT_OF_OBJECT));
    }
    return EXIT_SUCCESS;
}