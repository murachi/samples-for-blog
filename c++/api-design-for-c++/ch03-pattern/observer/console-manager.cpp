/**
	@file	console-manager.cpp
	@brief	コンソール管理オブジェクトクラス実装
*/

#include "console-manager.h"

#include <memory>
#include <iostream>
#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#else	//_WIN32
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#endif	//_WIN32

namespace apides {

struct ConsoleManager::Impl {
#ifdef _WIN32
	HANDLE console_handle;
#endif	//_WIN32

	Impl()
	{
#ifdef _WIN32
		// 標準出力のハンドルを取得
		console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
		// コンソールをクリア
		COORD coord{0, 0};
		DWORD count;
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(console_handle, &csbi);
		FillConsoleOutputCharactor(hStdOut, ' ', csbi.dwSize.X * csbi.dwSize.Y,
			coord, &count);
#else	//_WIN32
		// コンソールをクリア
		std::cout << "\x1b[2J" << std::flush;
#endif	//_WIN32
	}
	~Impl()
	{
#ifdef _WIN32
		// 色だけ戻す
		SetConsoleTextAttribute(console_handle,
			FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else	//_WIN32
		// 色だけ戻す
		std::cout << "\x1b[39m";
#endif	//_WIN32
		std::cout << std::endl;
	}
};

ConsoleManager::ConsoleManager()
	: impl{new ConsoleManager::Impl{}}
{}

ConsoleManager::~ConsoleManager() = default;

ConsoleManager::ConsoleSize ConsoleManager::getSize() const
{
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(console_handle, &csbi);
	return ConsoleSize{static_cast<int>(csbi.dwSize.X), static_cast<int>(csbi.dwSize.Y)};
#else	//_WIN32
	struct winsize winsz;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &winsz);
	return ConsoleSize{static_cast<int>(winsz.ws_col), static_cast<int>(winsz.ws_row)};
#endif	//_WIN32
}

void ConsoleManager::output(ConsoleManager::OutputInfo const& info) const
{
#ifdef _WIN32
	// カーソル位置
	COORD coord{static_cast<SHORT>(info.x), static_cast<SHORT>(info.y)};
	SetConsoleCursorPosition(console_handle, coord);
	// 文字色
	SetConsoleTextAttribute(console_handle,
		(info.color & 1 ? FOREGROUND_RED : 0) |
		(info.color & 2 ? FOREGROUND_GREEN : 0) |
		(info.color & 4 ? FOREGROUND_BLUE : 0));
#else	//_WIN32
	// カーソル位置、文字色
	std::cout << "\x1b[" << info.y << ";" << info.x << "H";
#endif	//_WIN32
	// テキスト本体を出力
	std::cout << info.text << std::flush;
}

int ConsoleManager::waitKeyInput() const
{
#ifdef _WIN32
#else	//_WIN32
	using TermIos = struct termios;
	TermIos pre;
	tcgetattr(STDIN_FILENO, &pre);
	TermIos raw = pre;
	cfmakeraw(&raw);
	tcsetattr(STDIN_FILENO, 0, &raw);
	auto term_restorer = [](TermIos * p){ tcsetattr(STDIN_FILENO, 0, p); };
	std::unique_ptr<TermIos, decltype(term_restorer)> tc_restorer{&pre, term_restorer};

	fd_set rdfs;
	FD_ZERO(&rdfs);
	FD_SET(0, &rdfs);

	auto retval = select(1, &rdfs, nullptr, nullptr, nullptr);
	if (retval == -1) {
		std::cerr << "select error" << std::endl;
		return -1;
	}
	return std::fgetc(stdin);
#endif	//_WIN32
}

bool operator==(ConsoleManager::OutputInfo const& lhs, ConsoleManager::OutputInfo const& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.text == rhs.text && lhs.color == rhs.color;
}

}	//namespace apides
