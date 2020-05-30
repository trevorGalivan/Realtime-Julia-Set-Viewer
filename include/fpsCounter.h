#pragma once
// A simple class to do fps calculations. Platform and framework agnostic

// Uses a "running average" to calculate the framerate, so more recent frames are weighted more than older frames
// This is more sensitive to spikes, and uses less memory than the "Average the last 100 frame intervals" method
class FpsCounter
{
public:
	FpsCounter();

	void update(double timeInSeconds);
	double getFPS() const;

private:
	double runningAvg;
	double lastTime;
};

