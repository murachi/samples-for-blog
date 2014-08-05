/**
	@file	console-view.h
	@brief	サブジェクトとして動作するコンソール表示ビュー抽象クラスの宣言
*/

#ifndef APIDES_CONSOLE_VIEW_H
#define APIDES_CONSOLE_VIEW_H

#include "subject.h"
#include "console-manager.h"

#include <memory>
#include <string>
#include <chrono>

namespace apides {

class ConsoleView : public SubjectBase {
	struct Impl;
	std::unique_ptr<Impl> impl;

public:
	enum ViewMessage {
		vm_Init = 100,
		vm_Timer,
	};

	explicit ConsoleView(ConsoleManager const& manager);
	virtual ~ConsoleView();

	virtual void initialize();
	void setTimer(std::chrono::milliseconds const& ms);
	void stopTimer();
	void setInterval(std::chrono::milliseconds const& ms);
	void stopInterval();

	using ViewStatus = ConsoleManager::OutputInfo;
	void setStatus(std::function<void(ViewStatus &)> func);

	ConsoleManager::ConsoleSize getConsoleSize() const;
};

}	//namespace apides

#endif	//APIDES_CONSOLE_VIEW_H

