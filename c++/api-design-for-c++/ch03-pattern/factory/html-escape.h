/**
	@file	html-escape.h
	@brief	HTML エスケープ処理を行うクラス定義
*/
#ifndef APIDES_FACTORY_HTML_ESCAPE_H
#define APIDES_FACTORY_HTML_ESCAPE_H

#include "reversible-base.h"
#include <string>
#include <memory>

namespace apides {

class HtmlEscape : public ReversibleBase {
public:
	HtmlEscape();
	virtual ~HtmlEscape();

	virtual std::string encode(std::string const& text) const;
	virtual std::string decode(std::string const& text) const;

private:
	class Impl;
	std::unique_ptr<Impl> impl;

	struct FactoryRegister;
	static FactoryRegister factory_register;
};

}	//namespace apides

#endif	//APIDES_FACTORY_HTML_ESCAPE_H
