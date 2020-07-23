#include "stdafx.h"
#include "Timer.h"

Timer g_Timer;

Timer::Timer() : lockedTime(0.0f)
{
	Reset();
}

void Timer::Reset()
{
	if (!QueryPerformanceCounter(&lastUpdate))
	{
		_ASSERT_EXPR(false, TEXT("QueryPerformanceCounter() failed."));
	}

	if (!QueryPerformanceFrequency(&frequency))
	{
		_ASSERT_EXPR(false, TEXT("QueryPerformanceCounter() failed."));
	}
}

float Timer::Elapsed()
{
	LARGE_INTEGER current;
	if (!QueryPerformanceCounter(&current))
	{
		_ASSERT_EXPR(false, "QueryPerformanceCounter() failed.");
	}

	return static_cast<float>(current.QuadPart - lastUpdate.QuadPart) / frequency.QuadPart;
}

float Timer::Lock()
{
	lockedTime = Elapsed();
	return lockedTime;
}

float Timer::ElapsedSinceLock()
{
	return Elapsed() - lockedTime;
}

float Timer::GetLockedTime()
{
	return lockedTime;
}
