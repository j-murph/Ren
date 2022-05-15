#pragma once
#include "Timer.h"

inline void AllocDebugConsole()
{
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);
}

template<class T>
inline float BenchmarkFunction(T&& func)
{
	Timer timer;
	func();
	return timer.Elapsed();
}