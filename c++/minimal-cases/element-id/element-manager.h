/**
	@file	element-manager.h
	@brief	要素 ID をプールし管理するクラス定義
*/
#ifndef MINIMUM_ELEMENT_MANAGER_H
#define MINIMUM_ELEMENT_MANAGER_H

#include <memory>
#include <string>
#include <boost/noncopyable.hpp>

namespace minimum {

/**
	@brief	要素 ID をプールし管理するクラス
*/
class ElementManager : private boost::noncopyable {
	struct Impl;
	std::unique_ptr<Impl> impl;	///< 実装オブジェクト

public:
	/**
		@brief	デフォルトコンストラクタ
	*/
	ElementManager();
	/**
		@brief	デストラクタ
	*/
	~ElementManager();
	/**
		@brief	要素と ID を登録する。
		@param[in]	elem		要素
		@param[in]	elem_id		要素 ID
		@return	実際に登録された要素 ID。

		@p elem_id を省略した場合、ユニークな ID を自動生成する。
		また、指定された @p elem_id の要素がすでに存在する場合、
		ID 文字列にハイフン付き番号 ("-1" のような) のサフィックスをつけて登録する。
		いずれの場合も、生成もしくは修正された ID 文字列を戻り値として返す。
	*/
	std::string registElement(std::shared_ptr<Element> elem, std::string const& elem_id = "");
	/**
		@brief	ID を指定して要素を取得する。
		@param[in]	elem_id		要素 ID
		@return	指定された ID の要素があればその共有ポインタを返す。なければ空の共有ポインタを返す。
	*/
	std::shared_ptr<Element> getElement(std::string const& elem_id) const;
};

}	//namespace minimum

#endif	//MINIMUM_ELEMENT_MANAGER_H
