/**
	@file	console-view.cpp
	@brief	サブジェクトとして動作するコンソール表示ビュー抽象クラスの実装
*/

#include "console-view.h"
#include "console-manager.h"
#include <thread>
#include <mutex>

namespace apides {

struct ConsoleView::Impl {
	ConsoleManager const* console;
	ViewStatus status;

	std::thread timer_thread;
	std::thread interval_thread;
	bool want_stop_timer;
	bool want_stop_interval;
	std::mutex mutex_want_stop_timer;
	std::mutex mutex_want_stop_interval;

	explicit Impl(ConsoleManager const& manager) :
		console{&manager}, status{0, 0, std::string{}, 7},
		timer_thread{}, interval_thread{}, want_stop_timer{false}, want_stop_interval{false}
	{}
	~Impl();
	void remove() const
	{
		std::string space(status.text.size(), ' ');
		console->output(ConsoleManager::OutputInfo{status.x, status.y, space, 7});
	}
	void draw() const
	{
		console->output(status);
	}
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

ConsoleView::ConsoleView(ConsoleManager const& manager) :
	impl{new ConsoleView::Impl{manager}}
{
}

ConsoleView::~ConsoleView() = default;

void ConsoleView::initialize()
{
	notifyObserver(vm_Init);
}

void ConsoleView::setTimer(std::chrono::milliseconds const& ms)
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

void ConsoleView::setInterval(std::chrono::milliseconds const& ms)
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

