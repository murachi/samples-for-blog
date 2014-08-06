/**
	@file	main.cpp
	@brief	オブザーバーパターンを用いたコンソール UI の MVC 実装サンプル
*/

#include "console-manager.h"
#include "static-text.h"
#include "key-listener.h"
#include "watch-controller.h"
#include "wait-controller.h"

int main()
{
	apides::ConsoleManager console;
	auto size = console.getSize();
	auto xpos = size.width / 2 - 9;
	apides::StaticText text_view{console, xpos > 0 ? xpos : 0, size.height / 2, 7, ""};
	apides::WatchController controller{text_view};
	text_view.initialize();

	apides::KeyListener listener{console};
	apides::WaitController waiter{listener};
	listener.initialize();	// キー入力があるまでブロック

	return 0;
}
