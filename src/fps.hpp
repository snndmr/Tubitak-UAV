#ifndef FPS_HPP
#define FPS_HPP

#include <time.h>

using namespace std;

class FPS {
   public:
    FPS();
    ~FPS();

    double mClock();
    double calcAvgFps();
    double calcAvgDur(double);
    double getAvarageFps();

   private:
    double fps1Sec;
    double fpsStart;
    double avarageFps;
    double avarageDuration;
    struct timespec mTimeSpec;
};

#endif