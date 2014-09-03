/**
	@file	element.cpp
	@brief	楽曲を構成する要素の共通インタフェース実装
*/

#include "element.h"

namespace minimum {

struct Element::Impl {
	std::string element_id;
};

Element::Element() :
	impl{new Element::Impl{}}
{}

Element::~Element() = default;

void Element::setId(std::string const& elem_id)
{
	impl->element_id = elem_id;
}

std::string const& Element::getId() const
{
	return impl->element_id;
}

}	//namespace minimum
