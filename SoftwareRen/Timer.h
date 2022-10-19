#pragma once
#include <chrono>

class Timer
{
private:
	std::chrono::steady_clock clock;
	std::chrono::time_point<std::chrono::steady_clock> lastUpdate;
	float lockedTime;

public:
	Timer();

	void Reset();
	float Elapsed();
	float Lock();
	float GetLockedTime();
};

extern Timer g_Timer;