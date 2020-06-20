#pragma once

#include <time.h>

class FPS {
	private:

	double fps1Sec;
	double fpsStart;
	double avarageFps;
	double avarageDuration;

	public:

	FPS();
	~FPS();

	double calcAvgFps();
	double calcAvgDur(double);
	double getAvarageFps();
};

