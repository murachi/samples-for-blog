/**
	@file	autotimer.cpp
	@brief	自動タイマーAPI 実装

	Pimpl イディオムサンプル。
*/
#include "autotimer.h"
#include <iostream>

#if _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

namespace apides {

class AutoTimer::Impl
{
public:
	double GetElapsed() const
	{
#ifdef _WIN32
		return (GetTickCount() - start_time) / 1e3;
#else
		struct timeval end_time;
		gettimeofday(&end_time, nullptr);
		double t1 = start_time.tv_usec / 1e6 + start_time.tv_sec;
		double t2 = end_time.tv_usec / 1e6 + end_time.tv_sec;
		return t2 - t1;
#endif	//_WIN32
	}

	std::string name;
#ifdef _WIN32
	DWORD start_time;
#else
	struct timeval start_time;
#endif	//_WIN32
};

AutoTimer::AutoTimer(const std::string &name) :
	impl(new AutoTimer::Impl())
{
	impl->name = name;
#ifdef _WIN32
	impl->start_time = GetTickCount();
#else
	gettimeofday(&impl->start_time, nullptr);
#endif	//_WIN32
}

AutoTimer::~AutoTimer()
{
	std::cout << impl->name << ": took " << impl->GetElapsed() << " secs" << std::endl;
}

}	// namespace apides

