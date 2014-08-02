/**
	@file	watch-controler.cpp
	@brief	時計表示処理コントローラオブジェクトクラス実装
*/

#include "watch-controler.h"
#include "datetime.h"
#include "static-text.h"

#include <chrono>

namespace apides {

struct WatchControler::Impl {
	DateTime datetime;
	StaticText * text_view;

	Impl(StaticText & view) : datetime{}, text_view{&view} {}
};

WatchControler::WatchControler(StaticText & text_view)
	: impl{new WatchControler::Impl{text_view}}
{
	text_view.subscribe(ConsoleView::vm_Init,
		std::shared_ptr<ObserverBase>{this, [](ObserverBase *){}});
	text_view.subscribe(ConsoleView::vm_Timer,
		std::shared_ptr<ObserverBase>{this, [](ObserverBase *){}});
}

WatchControler::~WatchControler() = default;

void WatchControler::notify(int message)
{
	switch (message) {
	case ConsoleView::vm_Init:
		impl->text_view->setInterval(std::chrono::milliseconds{50});
		break;
	case ConsoleView::vm_Timer:
		impl->text_view->changeText(impl->datetime.updateNow().toString());
		break;
	}
}

}	//namespace apides
