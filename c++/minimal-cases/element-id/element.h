/**
	@file	element.h
	@brief	楽曲を構成する要素の共通インタフェース定義
*/
#ifndef MINIMUM_ELEMENT_H
#define MINIMUM_ELEMENT_H

namespace minimum {

/**
	@brief	楽曲を構成する要素の共通インタフェース
*/
class Element {
public:
	/**
		@brief	デストラクタ
	*/
	virtual ~Element() = default;
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
