#include "camera.hpp"
#include "map.hpp"

void render(Mat image, vector<Point> marks) {
    Mat instant = image.clone();

    for (Point mark : marks) drawMarker(instant, mark, Scalar(100, 255, 255), MARKER_TRIANGLE_DOWN, 10, 2);
    polylines(instant, marks, true, Scalar(255, 255, 100), 1);

    imshow(WIN_NAME_MAP, instant);
}

void onMouse(int event, int x, int y, int, void *userdata) {
    // 1: EVENT_LBUTTONDOWN, 2: EVENT_RBUTTONDOWN, 3: EVENT_MBUTTONDOWN
    if (event > 0 && event < 4) {
        Map *map = (Map *)userdata;

        if (event == EVENT_MBUTTONDOWN) map->getMarks().clear();
        if (event == EVENT_LBUTTONDOWN) map->getMarks().push_back(Point(x, y));
        if (event == EVENT_RBUTTONDOWN && !map->getMarks().empty()) map->getMarks().pop_back();

        render(map->getOriginal(), map->getMarks());
    }
}

int main(int argc, char **argv) {
    if (argc != 2) {
        cout << "Error: Missing args";
        return EXIT_FAILURE;
    }

    Mat image = imread(argv[1]);
    if (image.empty()) {
        cout << "Error: Could not open or load " << argv[1];
        return EXIT_FAILURE;
    }

    Map map(image);
    namedWindow(WIN_NAME_MAP, WINDOW_NORMAL);
    resizeWindow(WIN_NAME_MAP, Size(image.cols / 1.4, image.rows / 1.4));
    setMouseCallback(WIN_NAME_MAP, onMouse, &map);

    char pressedKey = '\0';
    while (pressedKey != 27) {
        imshow(WIN_NAME_MAP, map.getOriginal());

        pressedKey = waitKey(0);
        if (pressedKey == 32 && !map.getMarks().empty()) {
            map.getMarks().push_back(map.getMarks().at(0));
            Camera camera(map);
            map.getMarks().clear();
        }
    }
    return EXIT_SUCCESS;
}