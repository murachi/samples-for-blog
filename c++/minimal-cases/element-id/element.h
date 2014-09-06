/**
	@file	element.h
	@brief	楽曲を構成する要素の共通インタフェース定義
*/
#ifndef MINIMUM_ELEMENT_H
#define MINIMUM_ELEMENT_H

#include <memory>
#include <string>

namespace minimum {

/**
	@brief	楽曲を構成する要素の共通インタフェース
*/
class Element {
	struct Impl;
	std::unique_ptr<Impl> impl;

public:
	/**
		@brief	コンストラクタ
	*/
	Element();
	/**
		@brief	デストラクタ
	*/
	virtual ~Element();
	/**
		@brief	要素 ID を設定する
		@param[in]	elem_id		要素 ID

		ElementRegistry への登録を経て確定した ID を設定すること。
		ID の変更を行いたい場合は ElementId テンプレートクラスを使用するべき。
	*/
	void setId(std::string const& elem_id);
	/**
		@brief	要素 ID を取得する
		@return	要素 ID
	*/
	std::string const& getId() const;
	/**
		@brief	XML 形式の文字列に変換する
	*/
	virtual std::string toXml() const = 0;
	/**
		@brief	XML 形式の文字列からデータを生成する
		@param[in]	xml		XML 形式の文字列
	*/
	virtual void fromXml(std::string const&) = 0;
};

}	//namespace minimum

#endif	//MINIMUM_ELEMENT_H
