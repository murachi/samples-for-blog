/**
	@file	main.cpp
	@brief	ファクトリを利用するプログラムのサンプル。
*/

#include "reversible-factory.h"
#include <iostream>
#include <memory>
#include <string>

int main()
{
	std::unique_ptr<apides::ReversibleBase> html_escaper{apides::ReversibleFactory{}.CreateReversible("html escape")};

	std::string src_text = "This program is sample of Factory & C++11. Written by \"T.MURACHI\" <murachi +at+ er-keiyo.sakura.ne.jp>";
	auto escaped_text = html_escaper->encode(src_text);

	std::cout << "Source text: " << src_text << std::endl;
	std::cout << "HTML Escaped text: " << escaped_text << std::endl;

	auto repaired_text = html_escaper->decode(src_text);

	std::cout << "Repaired text: " << repaired_text << std::endl;

	return 0;
}
