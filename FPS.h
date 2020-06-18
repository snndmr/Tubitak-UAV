#pragma once
#include <time.h>

class FPS {
	private:
	double avgdur;
	double avgfps;
	double fps1sec;
	double fpsstart;

	public:
	FPS();
	~FPS();
	double averagedur(double);
	double averagefps();
};

