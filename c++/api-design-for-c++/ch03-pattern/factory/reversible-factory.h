/**
	@file	reversible-factory.h
	@brief	テキスト可逆変換オブジェクトファクトリクラス定義

	Factory パターンサンプル。
*/
#ifndef APIDES_FACTORY_REVERSIBLE_FACTORY_H
#define APIDES_FACTORY_REVERSIBLE_FACTORY_H

#include "noncopyable.h"
#include "reversible-base.h"

namespace apides {

class ReversibleFactory : private NonCopyable {
public:
	ReversibleBase * CreateReversible(std::string const& type);
};

}	//namespace apides

#endif	//APIDES_FACTORY_REVERSIBLE_FACTORY_H
