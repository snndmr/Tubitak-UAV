#define SONAR_ENABLE 0

#include "detector.hpp"
#include "fps.hpp"
#include "sonar.hpp"

const char* const WIN_NAME_MAIN = "Main";

void mPutText(Mat frame, string text, Point pos) {
    putText(frame, text, pos, FONT_ITALIC, 0.75, Scalar(255, 255, 255), 2);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Missing args: Specify the camera index");
        return EXIT_FAILURE;
    }

    VideoCapture videoCapture(argv[1]);
    if (!videoCapture.isOpened()) return EXIT_FAILURE;

#if SONAR_ENABLE
    if (wiringPiSetup() == -1) return EXIT_FAILURE;
    Sonar sonar(PIN_TRIG, PIN_ECHO);
#endif

    Detector* detect = new Detector();

    FPS fps;
    double beg;
    double dur;

    Mat frame;
    Bundle bundle;
    char pressedKey;

    for (int frameIndex = 1; pressedKey = waitKey(1); frameIndex++) {
        beg = fps.mClock();
        videoCapture.read(frame);

        if (pressedKey == 27) break;

        if (frame.empty()) {
            printf("\n Frame could not be readed!");
            return EXIT_FAILURE;
        }

        vector<Bundle> bundles = detect->process(frame);

        for (Bundle bundle : bundles) {
            switch (bundle.type) {
                case RECT:
                    mPutText(frame, format("Blue Rect: %d, %d", bundle.positon.x, bundle.positon.y), bundle.positon);
                    break;
                case SQUARE:
                    mPutText(frame, format("Blue Square: %d, %d", bundle.positon.x, bundle.positon.y), bundle.positon);
                    break;
                case CIRCLE:
                    mPutText(frame, format("Red Circle: %d, %d", bundle.positon.x, bundle.positon.y), bundle.positon);
                    break;
            }

            polylines(frame, bundle.approx, true, Scalar(255, 255, 255), 2);
        }

        dur = fps.mClock() - beg;

        mPutText(frame, format("Frame Per Second: %.2lf", fps.calcAvgFps()), Point(10, 30));
        mPutText(frame, format("Frame Rate : %.2lf ms.", fps.calcAvgDur(dur)), Point(10, 60));
        mPutText(frame, format("Frame: %d", frameIndex), Point(10, 90));
#if SONAR_ENABLE
        mPutText(frame, format("Distance: %.2lf cm.", sonar.distance(30000)), Point(10, 120));
#endif
        imshow(WIN_NAME_MAIN, frame);
    }

    return EXIT_SUCCESS;
}
