/**
	@file	watch-view.h
	@brief	時計を表示するコンソールビューオブジェクトクラス宣言
*/

#ifndef APIDES_WATCH_VIEW_H
#define APIDES_WATCH_VIEW_H

#include "console-view.h"
#include <memory>
#include <boost/noncopyable.hpp>

namespace apides {

class DateTime;

class WatchView : public ConsoleView, private boost::noncopyable {
	struct Impl;
	std::unique_ptr<Impl> impl;

	WatchView();
	explicit WatchView(DateTime dt);
	virtual ~WatchView();
};

}	//namespace apides

#endif	//APIDES_WATCH_VIEW_H
