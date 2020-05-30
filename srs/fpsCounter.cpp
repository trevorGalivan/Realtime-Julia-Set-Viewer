#include "fpsCounter.h"

FpsCounter::FpsCounter() : runningAvg(0.), lastTime(0.) {}

void FpsCounter::update(double timeInSeconds) {
	runningAvg = runningAvg * 0.8 + 0.2 / (timeInSeconds - lastTime);
	lastTime = timeInSeconds;
}

double FpsCounter::getFPS() const {
	return runningAvg;
}
