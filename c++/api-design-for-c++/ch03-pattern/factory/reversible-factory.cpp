/**
	@file	reversible-factory.cpp
	@brief	テキスト可逆変換オブジェクトファクトリクラス実装
*/

#include "reversible-factory.h"

#include "html-escape.h"
#include "base64.h"

namespace apides {

ReversibleBase * ReversibleFactory::CreateReversible(std::string const& type)
{
	if (type == "html escape")	return new HtmlEscape();
	if (type == "base64") return new Base64Converter();

	return nullptr;
}

}	//namespace apides

