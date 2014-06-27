/**
	@file	base64.h
	@brief	BASE64変換処理を行うクラス定義
*/
#ifndef APIDES_FACTORY_BASE64_H
#define APIDES_FACTORY_BASE64_H

#include "reversible-base.h"
#include <memory>
#include <exception>

namespace apides {

class Base64Converter : public ReversibleBase {
public:
	class Base64ConverterException : public std::exception {
		char const* message;
		Base64ConverterException() = delete;
	public:
		explicit Base64ConverterException(char const* msg) noexcept :
			message{msg}
		{}
		virtual ~Base64ConverterException() = default;
		virtual char const* what() const noexcept { return message; }
	};

private:
	class Base64Map;
	static Base64Map const map;

public:
	Base64Converter();
	virtual ~Base64Converter();

	virtual std::string encode(std::string const& text) const;
	virtual std::string decode(std::string const& text) const;

private:
	struct Impl;
	std::unique_ptr<Impl> impl;

	struct FactoryRegister;
	static FactoryRegister factory_register;
};

}	//namespace apides

#endif	//APIDES_FACTORY_BASE64_H
