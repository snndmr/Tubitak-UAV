#include "FPS.h"

FPS::FPS() : avarageDuration(0), avarageFps(0), fps1Sec(0), fpsStart(0) {}

double FPS::calcAvgFps() {
	if(clock() - fpsStart >= 1000) {
		fpsStart = clock();
		avarageFps = 0.7 * avarageFps + 0.3 * fps1Sec;
		fps1Sec = 0;
	}
	fps1Sec += 1;
	return avarageFps;
}

double FPS::calcAvgDur(double newdur) { return 0.98 * avarageDuration + 0.02 * newdur; }

double FPS::getAvarageFps() { return avarageFps; }

FPS::~FPS() {}

