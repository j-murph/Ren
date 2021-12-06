#include "stdafx.h"
#include "Timer.h"

Timer g_Timer;

Timer::Timer() : lockedTime(0.0f)
{
	Reset();
}

void Timer::Reset()
{
	lastUpdate = clock.now();
}

float Timer::Elapsed()
{
	auto elapsed = clock.now() - lastUpdate;
	auto elapsedMicrosenconds = std::chrono::duration_cast<std::chrono::microseconds>(clock.now() - lastUpdate);
	return elapsedMicrosenconds.count() / 1000000.0f;
}

float Timer::Lock()
{
	lockedTime = Elapsed();
	return lockedTime;
}

float Timer::GetLockedTime()
{
	return lockedTime;
}
