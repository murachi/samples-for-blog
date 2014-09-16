/**
	@file	composit-header.cpp
	@brief	楽曲のヘッダー情報クラス実装
*/

#include "composit-header.h"

namespace minimum {

/**
	@brief	CompositHeader クラスの実装オブジェクトクラス
*/
struct CompositHeader::Impl {
};

CompositHeader::CompositHeader() :
	Element{}, impl{new CompositHeader::Impl{}}
{}

CompositHeader::~CompositHeader() = default;

}	//namespace minimum
