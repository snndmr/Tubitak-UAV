#include "fps.hpp"

FPS::FPS() : avarageDuration(0), avarageFps(0), fps1Sec(0), fpsStart(0) {}

FPS::~FPS() {}

double FPS::mClock() {
    clock_gettime(CLOCK_MONOTONIC, &mTimeSpec);
    return (mTimeSpec.tv_sec * 1000) + (mTimeSpec.tv_nsec * 1e-6);
}

double FPS::calcAvgFps() {
    if (mClock() - fpsStart >= 1000) {
        fpsStart   = mClock();
        avarageFps = 0.7 * avarageFps + 0.3 * fps1Sec;
        fps1Sec    = 0;
    }
    fps1Sec += 1;
    return avarageFps;
}

double FPS::calcAvgDur(double newdur) { return 0.98 * avarageDuration + 0.02 * newdur; }

double FPS::getAvarageFps() { return avarageFps; }