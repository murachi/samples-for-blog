/**
	@file	aho.cpp
	@brief	あほプログラム

	autotimer API を使ってあほ出力にかかる時間を計測するサンプル。
*/

#include <iostream>
#include <string>
#include "autotimer.h"

void aho(int count)
{
	apides::AutoTimer auto_timer("aho");

	for (int i = 0; i < count; i++) {
		std::cout << "あほ ";
	}

	std::cout << std::endl;
}

int main()
{
	aho(65536);

	return 0;
}
