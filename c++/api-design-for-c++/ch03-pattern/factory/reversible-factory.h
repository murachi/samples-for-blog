/**
	@file	reversible-factory.h
	@brief	テキスト可逆変換オブジェクトファクトリクラス定義

	Factory パターンサンプル。
*/
#ifndef APIDES_FACTORY_REVERSIBLE_FACTORY_H
#define APIDES_FACTORY_REVERSIBLE_FACTORY_H

#include "noncopyable.h"
#include "reversible-base.h"

#include <string>

namespace apides {

class ReversibleFactory : private NonCopyable {
public:
	ReversibleBase * CreateReversible(std::string const& type);

	typedef ReversibleBase *(*FactoryCallback)();
	static void registFactory(std::string const& type_name, FactoryCallback callback);
};

}	//namespace apides

#endif	//APIDES_FACTORY_REVERSIBLE_FACTORY_H
