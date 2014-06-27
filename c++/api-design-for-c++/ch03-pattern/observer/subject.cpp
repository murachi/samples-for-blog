/**
	@file	subject.cpp
	@brief	サブジェクトオブジェクトの抽象クラス実装
*/

#include "subject.h"
#include "observer.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <algorithm>

namespace apides {

struct SubjectBase::Impl {
	typedef std::vector<std::shared_ptr<ObserverBase>> ObserverList;
	typedef std::unordered_map<int, ObserverList> ObserverMap;
	ObserverMap bank;
	std::mutex mutex;
};

SubjectBase::SubjectBase() :
	impl{new SubjectBase::Impl{}}
{}

SubjectBase::~SubjectBase() = default;

void SubjectBase::subscribe(int message, std::shared_ptr<ObserverBase> observer)
{
	std::lock_guard<std::mutex> lock{impl->mutex};

	if (impl->bank.find(message) == impl->bank.end())
		impl->bank[message] = Impl::ObserverMap{};
	impl->bank[message].push_back(observer);
}

void SubjectBase::unsubscribe(int message, std::shared_ptr<ObserverBase> observer)
{
	std::lock_guard<std::mutex> lock{impl->mutex};

	if (impl->bank.find(message) == impl->bank.end())
		throw UnsubscribeException{"Message not found"};
	auto target = std::find_if(impl->bank[message].begin(), impl->bank[message].end(),
		[observer](auto elem){ return elem == observer; });
	if (target == impl->bank[message].end())
		throw UnsubscribeException{"Observer not found"};

	impl->bank[message].erase(target);
}

void SubjectBase::notify(int message)
{
	std::lock_guard<std::mutex> lock{impl->mutex};
}

}	//namespace apides

