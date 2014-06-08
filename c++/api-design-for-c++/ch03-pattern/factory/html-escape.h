/**
	@file	html-escape.h
	@brief	HTML エスケープ処理を行うクラス定義
*/
#ifndef APIDES_FACTORY_HTML_ESCAPE_H
#define APIDES_FACTORY_HTML_ESCAPE_H

#include "reversible-base.h"

namespace apides {

class HtmlEscape : public ReversibleBase {
public:
	virtual ~HtmlEscape();

	virtual std::string encode(std::string const& text) const;
	virtual std::string decode(std::string const& text) const;
};

}	//namespace apides

#endif	//APIDES_FACTORY_HTML_ESCAPE_H
