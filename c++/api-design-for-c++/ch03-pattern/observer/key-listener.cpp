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
	void listen(SubjectBase * self)
	{
		listener_thread = std::thread([=](){
			key_buffer.push(console->waitKeyInput());
			self->notifyObserver(vm_KeyInput);
		});
	}
};

KeyListener::Keylistener(ConsoleManager const& manager)
	: ConsoleView{manager}, impl{new Impl{manager}}
{}

KeyListener::~KeyListener() = default;

void KeyListener::initialize()
{
	impl->listen(this);
	ConsoleView::initialize();
}

int KeyLIstener::getKeyChar(bool wants_more)
{
	int c = impl->key_buffer.front();
	impl->key_buffer.pop();
	if (wants_more) impl->listen(this);
	return c;
}

}	//namespace apides
