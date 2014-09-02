/**
	@file	element-manager.cpp
	@brief	要素 ID をプールし管理するクラス実装
*/

#include "element-manager.h"

namespace minimum {

/**
	@brief	ElementManager クラスの実装オブジェクトクラス
*/
struct ElementManager::Impl {
};

ElementManager::ElementManager() :
	impl{new ElementManager::Impl{}}
{}

ElementManager::~ElementManager() = default;

}	//namespace minimum
