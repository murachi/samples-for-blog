/**
	@file	element-id.h
	@brief	要素 ID を扱うクラス定義
*/
#ifndef MINIMUM_ELEMENT_ID_H
#define MINIMUM_ELEMENT_ID_H

#include "element-manager.h"

#include <string>

namespace minimum {

/**
	@brief	要素 ID を扱うクラス
	@param	T	ID が指す要素の実際のクラス。
*/
template <class T>
class ElementId {
	/// 要素 ID
	std::string element_id;
	/// 要素 ID マネージャ
	ElementManager const* manager;

	/// コピーコンストラクタ (使用禁止)
	ElementId(ElementId const&) = delete;
	/// コピー代入演算子 (使用禁止)
	ElementId & operator=(ElementId const&) = delete;
	/// ムーブコンストラクタ (使用禁止)
	ElementId(ElementId &&) = delete;
	/// ムーブ代入演算子 (使用禁止)
	ElementId & operator=(ElementId &&) = delete;
	/// デフォルトコンストラクタ (使用禁止)
	ElementID() = delete;

public:
	/**
		@brief	コンストラクタ
		@param[in]	elem_id		要素 ID
	*/
	explicit ElementId(ElementManager const* mgr, std::string const& elem_id) :
		manager{mgr}, element_id{elem_id}
	{}
	/**
		@brief	デストラクタ
	*/
	~ElementId() = default;
	/**
		@brief	要素にアクセスする。
		@return	アクセスに成功すれば要素を指す共有ポインタを返す。
	*/
	std::shared_ptr<T> getElement() const
	{
		return std::dynamic_pointer_cast<T>(manager->getElement(element_id));
	}
	/**
		@brief	要素の ID を変更する。
		@param[in]	elem_id		変更を希望する要素 ID
		@return	実際に設定された要素 ID
	*/
	std::string modifyElementId(std::string const& elem_id)
	{
		element_id = manager->modifyElement(element_id, elem_id);
		manager->getElement(element_id)->setId(element_id);

		return element_id;
	}
};

}	//namespace minimum

#endif	//MINIMUM_ELEMENT_ID_H
