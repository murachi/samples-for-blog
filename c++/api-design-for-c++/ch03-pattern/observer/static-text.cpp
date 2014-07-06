/**
	@file	static-text.cpp
	@brief	静的テキスト表示ビューオブジェクトクラス実装
*/

#include "static-text.h"

namespace apides {

struct StaticText::Impl {
};

StaticText::StaticText(int x, int y, int color, std::string const& text) :
	ConsoleView{}
{
	setStatus([x, y, color, const& text](ViewStatus &stat){
		stat.x = x;
		stat.y = y;
		stat.color = color;
		stat.text = text;
	});
}

StaticText::~StaticText() = default;

void StaticText::changeText(std::string const& text)
{
	setStatus([const& text](ViewStatus &stat){
		stat.text = text;
	});
}

}	//namespace apides
