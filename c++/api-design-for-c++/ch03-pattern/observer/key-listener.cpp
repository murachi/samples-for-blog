/**
	@file	key-listener.cpp
	@brief	キー入力をエコーバックせずに受け取るビュークラス実装
*/

#include "key-listener.h"
#include "console-manager.h"

#include <queue>
#include <thread>

namespace apides {

struct KeyListener::Impl {
	std::queue<int> key_buffer;
	ConsoleManager const* console;
	std::thread listener_thread;

	explicit Impl(ConsoleManager const& manager) : console{&manager} {}
	~Impl()
	{
		if (listener_thread.joinable())
			listener_thread.detach();
	}
};

KeyListener::KeyListener(ConsoleManager const& manager)
	: ConsoleView{manager}, impl{new Impl{manager}}
{}

KeyListener::~KeyListener() = default;

void KeyListener::initialize()
{
	listen();
	ConsoleView::initialize();
}

int KeyListener::getKeyChar(bool wants_more)
{
	int c = impl->key_buffer.front();
	impl->key_buffer.pop();
	if (wants_more) listen();
	return c;
}

void KeyListener::listen()
{
	impl->listener_thread = std::thread([this](){
		impl->key_buffer.push(impl->console->waitKeyInput());
		notifyObserver(vm_KeyInput);
	});
}

}	//namespace apides
