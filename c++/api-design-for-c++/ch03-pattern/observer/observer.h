/**
	@file	observer.h
	@brief	オブザーバーオブジェクトの抽象クラス宣言
*/
#ifndef APIDES_OBSERVER_H
#define APIDES_OBSERVER_H

#include <boost/noncopyable.hpp>

namespace apides {

class ObserverBase : private boost::noncopyable {
public:
	virtual ~IObserver() = default;
	virtual void Update(int message) = 0;
};

}	//namespace apides

#endif	//APIDES_OBSERVER_H

