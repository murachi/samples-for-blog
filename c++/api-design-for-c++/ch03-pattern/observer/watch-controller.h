/**
	@file	watch-controller.h
	@brief	時計表示処理コントローラオブジェクトクラス宣言
*/
#ifndef APIDES_WATCH_CONTROLLER_H
#define APIDES_WATCH_CONTROLLER_H

#include "observer.h"
#include "static-text.h"

#include <memory>

namespace apides {

class WatchController : public ObserverBase {
	struct Impl;
	std::unique_ptr<Impl> impl;

public:
	explicit WatchController(StaticText &text_view);
	virtual ~WatchController();

	virtual void notify(int message) override;
};

}	//namespace apides

#endif	//APIDES_WATCH_CONTROLLER_H
