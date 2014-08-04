/**
	@file	console-manager.h
	@brief	コンソール画面管理オブジェクトクラス宣言

	シングルトンではなく、依存性の注入で利用するイメージ。
*/
#ifndef APIDES_CONSOLE_MANAGER_H
#define APIDES_CONSOLE_MANAGER_H

#include <memory>
#include <string>
#include <boost/noncopyable.hpp>

namespace apides {

class ConsoleManager : private boost::noncopyable {
	struct Impl;
	std::unique_ptr<Impl> impl;

public:
	ConsoleManager();
	~ConsoleManager();

	struct ConsoleSize {
		int width;
		int height;
	};
	ConsoleSize getSize() const;

	struct OutputInfo {
		int x;
		int y;
		std::string text;
		int color;
	};
	void output(OutputInfo const& info) const;

	int waitKeyInput() const;
};

extern bool operator==(ConsoleManager::OutputInfo const& lhs, ConsoleManager::OutputInfo const& rhs);

}	//namespace apides

#endif	//APIDES_CONSOLE_MANAGER_H
