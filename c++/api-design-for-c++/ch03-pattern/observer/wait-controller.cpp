/**
	@file	wait-controller.cpp
	@brief	キー入力待ちコントローラクラス実装
*/

#include "wait-controller.h"
#include "key-listener.h"

#include <thread>
#include <chrono>

namespace apides {

struct WaitController::Impl {
	KeyListener * listener;
	std::thread wait_thread;
	bool wants_finish;

	Impl(KeyListener & view) : listener{&view}, wants_finish{false} {}
};

WaitController::WaitController(KeyListener & listener)
	: impl{new WaitController::Impl{listener}}
{
	listener.subscribe(ConsoleView::vm_Init,
		std::shared_ptr<ObserverBase>{this, [](ObserverBase *){}});
	listener.subscribe(KeyListener::vm_KeyInput,
		std::shared_ptr<ObserverBase>{this, [](ObserverBase *){}});
}

WaitController::~WaitController() = default;

void WaitController::notify(int message)
{
	switch (message) {
	case ConsoleView::vm_Init:
		impl->listener->setStatus([this](ConsoleView::ViewStatus & status){
			status.text = "Hit any key to finish.";
			auto console_size = impl->listener->getConsoleSize();
			status.x = (console_size.width - status.text.size()) / 2;
			status.y = console_size.height - 2;
			status.color = 5;
		});

		impl->wait_thread = std::thread{[this](){
			while (!impl->wants_finish) {
				std::this_thread::sleep_for(std::chrono::milliseconds{50});
			}
		}};
		impl->wait_thread.join();
		break;
	case KeyListener::vm_KeyInput:
		int key = impl->listener->getKeyChar();
		impl->wants_finish = true;
		break;
	}
}

}	//namespace apides
