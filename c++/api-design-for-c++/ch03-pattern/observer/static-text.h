/**
	@file	static-text.h
	@brief	静的テキスト表示ビューオブジェクトクラス宣言
*/

#ifndef APIDES_STATIC_TEXT_H
#define APIDES_STATIC_TEXT_H

#include "console-view.h"
#include "console-manager.h"
#include <memory>
#include <string>

namespace apides {

class StaticText : public ConsoleView {
	struct Impl;
	std::unique_ptr<Impl> impl;

public:
	StaticText() = delete;
	StaticText(ConsoleManager const& manager, int x, int y, int color, std::string const& text);
	virtual ~StaticText();

	void changeText(std::string const& text);
};

}	//namespace spides

#endif	//APIDES_STATIC_TEXT_H
