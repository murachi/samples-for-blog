/**
	@file	wait-controller.h
	@brief	キー入力待ちコントローラクラス宣言
*/
#ifndef APIDES_WAIT_CONTROLLER_H
#define APIDES_WAIT_CONTROLLER_H

#include "observer.h"
#include "key-listener.h"

#include <memory>

namespace apides {

class WaitController : public ObserverBase {
	struct Impl;
	std::unique_ptr<Impl> impl;

public:
	WaitController(KeyListener & listener);
	virtual ~WaitController();

	virtual void notify(int message) override;
};

}	//namespace apides

#endif	//APIDES_WAIT_CONTROLLER_H
