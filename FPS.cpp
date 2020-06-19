#include "FPS.h"

FPS::FPS() : avgdur(0), avgfps(0), fps1sec(0), fpsstart(0) {}

double FPS::averagedur(double newdur) {
	return 0.98 * avgdur + 0.02 * newdur;
}

double FPS::averagefps() {
	if(clock() - fpsstart >= 1000) {
		fpsstart = clock();
		avgfps = 0.7 * avgfps + 0.3 * fps1sec;
		fps1sec = 0;
	}
	fps1sec++;
	return avgfps;
}

FPS::~FPS() {}

