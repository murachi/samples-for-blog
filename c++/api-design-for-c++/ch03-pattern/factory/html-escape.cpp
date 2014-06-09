/**
	@file	html-escape.cpp
	@brief	HTML エスケープ処理を行うクラスの実装
*/

#include "html-escape.h"
#include <string>
#include <algorithm>

namespace apides {

HtmlEscape::~HtmlEscape() = default;

std::string HtmlEscape::encode(std::string const& text) const
{
	std::string dist;
	auto isNeedEscape = [](char c) { return c == '<' || c == '>' || c == '&' || c == '"'; };
	auto from = text.begin();
	for (auto to = std::find_if(from, text.end(), isNeedEscape); to != text.end();
	to = std::find_if(from, text.end(), isNeedEscape)) {
		dist += std::string(from, to) + "&" +
			(*to == '<' ? "lt" :
			*to == '>' ? "gt" :
			*to == '&' ? "amp" :
			*to == '"' ? "quot" : "") + ";";
		from = to + 1;
	}
	dist += std::string(from, text.end());

	return dist;
}

std::string HtmlEscape::decode(std::string const& text) const
{
	std::string dist;
	auto from = text.begin();
	for (auto to = std::find(from, text.end(), '&'); to != text.end(); to = std::find(from, text.end(), '&')) {
		auto next_from = std::find(to, text.end(), ';');
		if (next_from == text.end()) break;
		auto pattern = std::string(to + 1, next_from);
		dist += std::string(from, to) +
			(pattern == "lt" ? "<" :
			pattern == "gt" ? ">" :
			pattern == "amp" ? "&" :
			pattern == "quot" ? "\"" : "");
		from = next_from + 1;
	}
	dist += std::string(from, text.end());

	return dist;
}

}	//namespace apides
