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

	std::thread wait_thread{[](){
		struct termios pre;
		tcgetattr(STDIN_FILENO, &pre);
		struct termios raw = pre;
		cfmakeraw(&raw);
		tcsetattr(STDIN_FILENO, 0, &raw);

		fd_set rdfs;
		FD_ZERO(&rdfs);
		FD_SET(0, &rdfs);
		struct timeval tv{0, 50};	// 50ms ごとにキー入力を監視

		// 何かしらキー入力があったら終了
		while (auto retval = select(1, &rdfs, nullptr, nullptr, &tv))
		{
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
