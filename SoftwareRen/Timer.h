#pragma once
#include <chrono>

using namespace std::chrono;

class Timer
{
private:
	steady_clock clock;
	time_point<std::chrono::steady_clock> lastUpdate;
	float lockedTime;

public:
	Timer();

	void Reset();
	float Elapsed();
	float Lock();
	float GetLockedTime();
};

extern Timer g_Timer;