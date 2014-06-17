/**
	@file	reversible-factory.cpp
	@brief	テキスト可逆変換オブジェクトファクトリクラス実装
*/

#include "reversible-factory.h"

#include "html-escape.h"
#include "base64.h"

namespace apides {

ReversibleFactory::FactoryMap ReversibleFactory::factory_map;

void ReversibleFactory::registFactory(std::string const* type_name, ReversibleFactory::FactoryCallback callback)
{
	factory_map[type_name] = callback;
}

ReversibleBase * ReversibleFactory::CreateReversible(std::string const& type)
{
	return factory_map.count(type) > 0 ? factory_map[type]() : nullptr;
}

}	//namespace apides

