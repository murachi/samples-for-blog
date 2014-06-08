/**
	@file	reversible-base.h
	@brief	テキストの可逆変換を行うオブジェクトの基本クラス
*/
#ifndef APIDES_FACTORY_REVERSIBLE_BASE_H
#define APIDES_FACTORY_REVERSIBLE_BASE_H

#include <string>
#include "noncopyable.h"

namespace apides {

class ReversibleBase : private NonCopyable {
public:
	virtual ~ReversibleBase() = default;

	virtual std::string encode(std::string const& text) const = 0;
	virtual std::string decode(std::string const& text) const = 0;
};

}	//namespace apides

#endif	//APIDES_FACTORY_REVERSIBLE_BASE_H
