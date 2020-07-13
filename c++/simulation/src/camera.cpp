#include "camera.hpp"

Camera::Camera(Map map) : map(map) {
    detector = new Detector();
    pos      = Point2d(map.getMarks().at(0));

    init();
}

Camera::~Camera() {}

void Camera::init() {
    for (unsigned int i = 0; i < map.getMarks().size(); i++) {
        bool isPaused   = false;
        char pressedKey = '\0';

        while (!isArrive(map.getMarks().at(i))) {
            pressedKey = waitKey(1);

            if (pressedKey == 27) break;
            if (pressedKey == 32) isPaused = !isPaused;
            if (isPaused) continue;

            moveTo(map.getOriginal(), map.getMarks().at(i));

            Mat instant = map.getOriginal()(getROI()).clone();
            Bundle data = detector->process(instant, pos);

            if (data.type == SQUARE && !isSquareFind) {
                grabble(map.getOriginal(), data);
            } else if (data.type == CIRCLE && !isCircleFind) {
                grabble(map.getOriginal(), data);
            } else {
                imshow(WIN_NAME_CAM, instant);
            }
        }
    }
}

void Camera::grabble(Mat image, Bundle bundle) {
    Point2d destOfObject(pos.x + (bundle.pos.x - sizeOfView.width / 2), pos.y + (bundle.pos.y - sizeOfView.height / 2));
    cout << destOfObject << " : " << bundle.pos << " : " << pos;
    int frameIndex  = 1;
    int rateOfCatch = 0;
    for (; !isArrive(destOfObject); frameIndex += 1) {
        moveTo(map.getOriginal(), destOfObject);
        Mat instant = map.getOriginal()(getROI()).clone();
        Bundle data = detector->process(instant, pos);

        if (data.type = bundle.type) {
            rateOfCatch += 1;
        }

        putText(instant, format("Rate of Catch: %d-%d (%.1lf%%)", rateOfCatch, frameIndex, rateOfCatch * 100.0 / frameIndex), Point(10, 90), FONT_HERSHEY_SIMPLEX, .6, CUSTOM_ORANGE, 2);
        imshow(WIN_NAME_CAM, instant);
        waitKey(1);
    }
    cout << endl << pos;

    if ((rateOfCatch * 100.0 / frameIndex) > 70) {
        if (bundle.type == SQUARE) {
            isSquareFind = true;
            cout << "\n Blue Square found at " << pos;
        } else if (bundle.type == CIRCLE) {
            isCircleFind = true;
            cout << "\n Red Circle found at " << pos;
        }
        waitKey(3000);
    }
}

void Camera::moveTo(Mat image, Point2d destination) {
    double eucliadian = norm(pos - destination);

    if (pos.x != destination.x) pos.x -= (pos.x - destination.x) / eucliadian;   // To find the number of steps of x
    if (pos.y != destination.y) pos.y -= (pos.y - destination.y) / eucliadian;   // To find the number of steps of y
}

Rect Camera::getROI() {
    Point2d beg(pos.x - sizeOfView.width / 2, pos.y - sizeOfView.height / 2);
    Point2d end(pos.x + sizeOfView.width / 2, pos.y + sizeOfView.height / 2);

    // To find the coordinates within the borders.
    if (beg.x < 0)
        beg.x = 0;
    else if (beg.x > map.getMapSize().width)
        beg.x = map.getMapSize().width;

    if (beg.y < 0)
        beg.y = 0;
    else if (beg.y > map.getMapSize().height)
        beg.y = map.getMapSize().height;

    if (end.x < 0)
        end.x = 0;
    else if (end.x > map.getMapSize().width)
        end.x = map.getMapSize().width;

    if (end.y < 0)
        end.y = 0;
    else if (end.y > map.getMapSize().height)
        end.y = map.getMapSize().height;

    return Rect(beg, end);
}

bool Camera::isArrive(Point2d destination) {
    return (pos.x + 1 > destination.x) && (pos.x - 1 < destination.x) &&
                   (pos.y + 1 > destination.y) && (pos.y - 1 < destination.y)
               ? true
               : false;
}