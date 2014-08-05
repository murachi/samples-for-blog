/**
	@file	key-listener.h
	@brief	キー入力をエコーバックせずに受け取るビュークラス宣言
*/
#ifndef APIDES_KEY_LISTENER_H
#define APIDES_KEY_LISTENER_H

#include "console-view.h"
#include "console-manager.h"

#include <memory>

namespace apides {

class KeyListener : public ConsoleView {
	struct Impl;
	std::unique_ptr<Impl> impl;

public:
	enum ViewMessage {
		vm_KeyInput = 200
	};

	explicit KeyListener(ConsoleManager const& manager);
	virtual ~KeyListener();

	virtual void initialize() override;

	int getKeyChar(bool wants_more = false);
};

}	//namespace apides

#endif	//APIDES_KEY_LISTENER_H
