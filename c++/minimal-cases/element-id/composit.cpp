/**
	@file	composit.cpp
	@brief	楽曲オブジェクトクラス実装
*/

#include "composit.h"

#include <utility>

namespace minimum {

/**
	@brief	Composit クラスの実装オブジェクトクラス
*/
struct Composit::Impl {
};

Composit::Composit() :
	Element{}, impl{new Composit::Impl{}}
{}

Composit::Composit(Composit && src) : impl{std::move(src.impl)}
{
	//TODO: ムーブ生成を行う処理の詳細をここに記述
}

Composit & Composit::operator=(Composit && src)
{
	this->impl = std::move(src.impl);
	//TODO: ムーブ代入を行う処理の詳細をここに記述
}

Composit::~Composit() = default;

std::string Composit::toXml() const
{
	//TODO: 子要素を XML 化する処理を記述

	return std::string("<Composit>\n") /* + sub_xml */ + "</Composit>\n";
}

void Composit::fromXml(std::string const& xml)
{
	//TODO: 子要素を生成する処理を記述
}

}	//namespace minimum
