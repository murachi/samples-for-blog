/**
	@file	base64.h
	@brief	BASE64変換処理を行うクラス定義
*/
#ifndef APIDES_FACTORY_BASE64_H
#define APIDES_FACTORY_BASE64_H

#include "reversible-base.h"
#include <exception>

namespace apides {

class Base64Converter : public ReversibleBase {
	class Base64Map {
		char map[64];
		char rmap[128];
	public:
		Base64Map();
		char n2b(char n) const;
		char b2n(char b) const;
	};

public:
	class Base64ConverterException : public std::exception {
		char const* message;
		Base64ConverterException() = delete;
	public:
		explicit Base64ConverterException(char const* msg) noexcept :
			message(msg)
		{}
		virtual ~Base64ConverterException() = default;
		virtual char const* what() const noexcept { return message; }
	};

private:
	static Base64Map const map;

public:
	virtual ~Base64Converter();

	virtual std::string encode(std::string const& text) const;
	virtual std::string decode(std::string const& text) const;
};

}	//namespace apides

#endif	//APIDES_FACTORY_BASE64_H
