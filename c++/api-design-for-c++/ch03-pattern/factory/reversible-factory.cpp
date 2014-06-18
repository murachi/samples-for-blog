/**
	@file	reversible-factory.cpp
	@brief	テキスト可逆変換オブジェクトファクトリクラス実装
*/

#include "reversible-factory.h"

#include "html-escape.h"
#include "base64.h"

#include <unordered_map>

namespace apides {

typedef std::unordered_map<std::string, ReversibleFactory::FactoryCallback> FactoryMap;

FactoryMap & getMap()
{
	static FactoryMap factory_map;
	return factory_map;
}

void ReversibleFactory::registFactory(std::string const& type_name, ReversibleFactory::FactoryCallback callback)
{
	getMap()[type_name] = callback;
}

ReversibleBase * ReversibleFactory::CreateReversible(std::string const& type)
{
	auto & factory_map = getMap();
	return factory_map.count(type) > 0 ? factory_map[type]() : nullptr;
}

}	//namespace apides

