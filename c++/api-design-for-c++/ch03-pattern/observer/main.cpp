/**
	@file	main.cpp
	@brief	オブザーバーパターンを用いたコンソール UI の MVC 実装サンプル
*/

#include "console-manager.h"
#include "static-text.h"
#include "watch-controler.h"

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
	apides::WatchControler controler{text_view};
	text_view.initialize();

	std::thread wait_thread{[](){
		using TermIos = struct termios;
		TermIos pre;
		tcgetattr(STDIN_FILENO, &pre);
		TermIos raw = pre;
		cfmakeraw(&raw);
		tcsetattr(STDIN_FILENO, 0, &raw);
		auto term_restorer = [](TermIos * p){ tcsetattr(STDIN_FILENO, 0, p); };
		std::unique_ptr<TermIos, decltype(term_restorer)> tc_restorer{&pre, term_restorer};

		fd_set rdfs;
		FD_ZERO(&rdfs);
		FD_SET(0, &rdfs);

		// 何かしらキー入力があったら終了
		for (;;)
		{
			auto retval = select(1, &rdfs, nullptr, nullptr, nullptr);
			if (retval == -1) {
				std::cerr << "select error" << std::endl;
				return;
			}
			struct timeval nonblock{0, 0};
			while (select(1, &rdfs, nullptr, nullptr, &nonblock)) {
				std::fgetc(stdin);
			}
			return;
		}
	}};
	wait_thread.join();

	return 0;
}
