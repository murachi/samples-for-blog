/**
	@file	main.cpp
	@brief	オブザーバーパターンを用いたコンソール UI の MVC 実装サンプル
*/

#include "console-manager.h"
#include "static-text.h"
#include "key-listener.h"
#include "watch-controller.h"
#include "wait-controller.h"

#include <thread>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>

#include <iostream>
#include <cstdio>

int main()
{
	apides::ConsoleManager console;
	auto size = console.getSize();
	auto xpos = size.width / 2 - 9;
	apides::StaticText text_view{console, xpos > 0 ? xpos : 0, size.height / 2, 2, ""};
	apides::WatchController controller{text_view};
	text_view.initialize();

	apides::KeyListener listener{console};
	apides::WaitController waiter{listener};
	listener.initialize();	// キー入力があるまでブロック

	return 0;
}
