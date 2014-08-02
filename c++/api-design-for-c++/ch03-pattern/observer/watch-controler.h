/**
	@file	watch-controler.h
	@brief	時計表示処理コントローラオブジェクトクラス宣言
*/
#ifndef APIDES_WATCH_CONTROLER_H
#define APIDES_WATCH_CONTROLER_H

#include "observer.h"
#include "static-text.h"

#include <memory>
#include <boost/noncopyable.hpp>

namespace apides {

class WatchControler : public ObserverBase {
	struct Impl;
	std::unique_ptr<Impl> impl;

public:
	explicit WatchControler(StaticText &text_view);
	virtual ~WatchControler();

	virtual void notify(int message) override;
};

}	//namespace apides

#endif	//APIDES_WATCH_CONTROLER_H
