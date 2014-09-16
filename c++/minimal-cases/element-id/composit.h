/**
	@file	composit.h
	@brief	楽曲オブジェクトクラス定義
*/
#ifndef MINIMUM_COMPOSIT_H
#define MINIMUM_COMPOSIT_H

#include "element.h"

#include <memory>

namespace minimum {

/**
	@brief	楽曲オブジェクトクラス
*/
class Composit : public Element {
	struct Impl;
	std::unique_ptr<Impl> impl;	///< 実装オブジェクト

	/// コピーコンストラクタ (使用禁止)
	Composit(Composit const& src) = delete;
	/// コピー代入演算子 (使用禁止)
	Composit & operator=(Composit const& src) = delete;

public:
	/**
		@brief	デフォルトコンストラクタ
	*/
	Composit();
	/**
		@brief ムーブコンストラクタ
	*/
	Composit(Composit && src);
	/**
		@brief ムーブ代入演算子
	*/
	Composit & operator=(Composit && src);
	/**
		@brief	デストラクタ
	*/
	virtual ~Composit();
	/**
		@brief	XML 形式の文字列に変換する
	*/
	virtual std::string toXml() const override;
	/**
		@brief	XML 形式の文字列からデータを生成する
		@param[in]	xml		XML 形式の文字列
	*/
	virtual void fromXml(std::string const& xml) override;
};

}	//namespace minimum

#endif	//MINIMUM_COMPOSIT_H
