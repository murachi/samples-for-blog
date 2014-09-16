/**
	@file	composit-header.h
	@brief	楽曲のヘッダー情報クラス定義
*/
#ifndef MINIMUM_COMPOSIT_HEADER_H
#define MINIMUM_COMPOSIT_HEADER_H

#include "element.h"

#include <memory>
#include <boost/noncopyable.hpp>

namespace minimum {

/**
	@brief	楽曲のヘッダー情報クラス
*/
class CompositHeader : public Element, private boost::noncopyable {
	struct Impl;
	std::unique_ptr<Impl> impl;	///< 実装オブジェクト

public:
	/**
		@brief	デフォルトコンストラクタ
	*/
	CompositHeader();
	/**
		@brief	デストラクタ
	*/
	virtual ~CompositHeader();
	/**
		@brief	XML 形式の文字列に変換する
	*/
	virtual std::string toXml() const override;
	/**
		@brief	XML 形式の文字列からデータを生成する
		@param[in]	xml		XML 形式の文字列
	*/
	virtual void fromXml(std::string const& xml) override;
	/**
		@brief	楽曲のタイトルにアクセスする
		@param[in]	new_title	新しいタイトル。変更しない場合は @c nullptr を指定する。
		@return	楽曲のタイトルを返す。未定義の場合は空文字列を返す。
	*/
	std::string const& Title(char const* new_title = nullptr) const;
	/**
		@brief	楽曲の制作者にアクセスする
		@param[in]	new_author	新しい制作者。変更しない場合は @c nullptr を指定する。
		@return	楽曲の制作者を返す。未定義の場合は空文字列を返す。
	*/
	std::string const& Author(char const* new_author = nullptr) const;
};

}	//namespace minimum

#endif	//MINIMUM_COMPOSIT_HEADER_H
