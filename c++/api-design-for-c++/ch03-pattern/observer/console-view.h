/**
	@file	console-view.h
	@brief	サブジェクトとして動作するコンソール表示ビュー抽象クラスの宣言
*/

#ifndef APIDES_CONSOLE_VIEW_H
#define APIDES_CONSOLE_VIEW_H

#include <string>
#include <chrono>

namespace apides {

class ConsoleView : public SubjectBase {
	struct Impl;
	std::unique_ptr<Impl> impl;

public:
	enum ViewMessage {
		vm_Init,
		vm_Timer,
	}

	struct ViewStatus {
		int x;
		int y;
		std::string text;
		int color;
	};

	ConsoleView();
	virtual ~ConsoleView();

	void initialize();
	void setTimer(std::chrono::milliseconds const& ms);
	void stopTimer();
	void setInterval(std::chrono::milliseconds const& ms);
	void stopInterval();

	void setStatus(std::function<void(ViewStatus &)> func);
};

}	//namespace apides

#endif	//APIDES_CONSOLE_VIEW_H

