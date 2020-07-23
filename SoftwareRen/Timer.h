#pragma once

class Timer
{
private:
	LARGE_INTEGER lastUpdate;
	LARGE_INTEGER frequency;

	float lockedTime;

public:
	Timer();

	void Reset();
	float Elapsed();
	float Lock();
	float ElapsedSinceLock();
	float GetLockedTime();
};

extern Timer g_Timer;