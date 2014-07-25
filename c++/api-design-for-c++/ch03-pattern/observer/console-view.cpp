/**
	@file	console-view.cpp
	@brief	サブジェクトとして動作するコンソール表示ビュー抽象クラスの実装
*/

#include "console-view.h"
#include <thread>
#include <mutex>

namespace apides {

struct ConsoleView::Impl {
	ViewState status;

	std::thread timer_thread;
	std::thread interval_thread;
	bool want_stop_timer;
	bool want_stop_interval;
	std::mutex mutex_want_stop_timer;
	std::mutex mutex_want_stop_interval;

	Impl() :
		status{0, 0, std::string{}, 7},
		timer_thread{}, interval_thread{}, want_stop_timer{false}, want_stop_interval{false}
	{}
	~Impl();
	void remove() const;
	void draw() const;
	void notifyStopTimer(bool is_stop = true)
	{
		std::lock_guard<std::mutex> lock(mutex_want_stop_timer);
		want_stop_timer = is_stop;
	}
	void notifyStopInterval(bool is_stop = true)
	{
		std::lock_guard<std::mutex> lock(mutex_want_stop_interval);
		want_stop_interval = is_stop;
	}
};

ConsoleView::Impl::~Impl()
{
	if (timer_thread.joinable()) {
		notifyStopTimer();
		timer_thread.join();
	}
	if (interval_thread.joinable()) {
		notifyStopInterval();
		interval_thread.join();
	}
}

void ConsoleView::Impl::remove() const
{
	// 実際の描画削除処理
}

void ConsoleView::Impl::draw() const
{
	// 実際の描画処理
}

ConsoleView::ConsoleView() :
	impl{new ConsoleView::Impl{}}
{
}

ConsoleView::~ConsoleView() = default;

void ConsoleView::initialize()
{
	notifyObserver(vm_Init);
}

void ConsoleView::setTimer(std::chrono::milliseconds ms)
{
	// 既存のタイマースレッドがある場合は、スレッドの終了を待機する
	if (impl->timer_thread.joinable()) {
		impl->notifyStopTimer();
		impl->timer_thread.join();
	}
	impl->timer_thread = std::thread([this, ms](){
		std::this_thread::sleep_for(ms);
		if (impl->want_stop_timer) {
			impl->notifyStopTimer(false);
			return;
		}
		notifyObserver(vm_Timer);
	});
}

void ConsoleView::stopTimer()
{
	impl->notifyStopTimer();
}

void ConsoleView::setInterval(std::chrono::milliseconds ms)
{
	// 既存のインターバルタイマースレッドがある場合は、スレッドの終了を待機する
	if (impl->interval_thread.joinable()) {
		impl->notifyStopInterval();
		impl->interval_thread.join();
	}
	impl->interval_thread = std::thread([this, ms](){
		std::this_thread::sleep_for(ms);
		while (!impl->want_stop_interval) {
			notifyObserver(vm_Timer);
			std::this_thread::sleep_for(ms);
		}
		impl->notifyStopInterval(false);
	});
}

void ConsoleView::stopInterval()
{
	impl->notifyStopInterval();
}

ConsoleView::ViewStatus ConsoleView::getStatus() const
{
	return impl->status;
}

void ConsoleView::setStatus(std::function<void(ConsoleView::ViewStatus &)> func)
{
	auto status = impl->status;
	func(status);
	if (status == impl->status) return;

	impl->remove();
	impl->status = status;
	impl->draw();
}

}	//namespace apides

