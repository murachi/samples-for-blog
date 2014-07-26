/**
	@file	static-text.cpp
	@brief	静的テキスト表示ビューオブジェクトクラス実装
*/

#include "static-text.h"

namespace apides {

struct StaticText::Impl {
};

StaticText::StaticText(ConsoleManager const& manager, int x, int y, int color, std::string const& text) :
	ConsoleView{manager}
{
	setStatus([&](ConsoleView::ViewStatus &stat){
		stat.x = x;
		stat.y = y;
		stat.color = color;
		stat.text = text;
	});
}

StaticText::~StaticText() = default;

void StaticText::changeText(std::string const& text)
{
	setStatus([&](ConsoleView::ViewStatus &stat){
		stat.text = text;
	});
}

}	//namespace apides
