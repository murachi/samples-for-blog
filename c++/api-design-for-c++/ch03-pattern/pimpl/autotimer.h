/**
	@file	autotimer.h
	@brief	自動タイマーAPI インタフェース

	Pimple イディオムサンプル。
*/

#ifndef APIDES_PIMPL_AUTOTIMER_H
#define APIDES_PIMPL_AUTOTIMER_H

#include <string>
#include <memory>

namespace apides {

class AutoTimer
{
	AutoTimer(const AutoTimer &) = delete;
	AutoTimer & operator=(const AutoTimer &) = delete;
	AutoTimer(AutoTimer &&) = delete;
	AutoTimer & operator=(AutoTimer &&) = delete;

public:
	explicit AutoTimer(const std::string &name);
	~AutoTimer();
private:
	class Impl;
	std::unique_ptr<Impl> impl;
};

}	// namespace apides

#endif	//APIDES_PIMPL_AUTOTIMER_H

