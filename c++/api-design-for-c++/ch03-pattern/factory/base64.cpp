/**
	@file	base64.cpp
	@brief	BASE64変換処理を行うクラスの実装
*/

#include "base64.h"
#include "reversible-factory.h"
#include <string>
#include <utility>
#include <algorithm>
#include <vector>
#include <array>
#include <iterator>
#include <cassert>

namespace apides {

class Base64Converter::Base64Map {
	char map[64];
	char rmap[128];
public:
	Base64Map();
	char n2b(char n) const;
	char b2n(char b) const;
};

Base64Converter::Base64Map::Base64Map()
{
	for (int i = 0; i < 26; i++) {
		map[i] = static_cast<char>('A' + i);
		map[26 + i] = static_cast<char>('a' + i);
	}
	for (int i = 0; i < 10; i++)
		map[52 + i] = static_cast<char>('0' + i);
	map[62] = '+';
	map[63] = '/';

	std::fill_n(rmap, sizeof(rmap), static_cast<char>(-1));
	for (int i = 0; i < 64; i++)
		rmap[map[i]] = static_cast<char>(i);
}

char Base64Converter::Base64Map::n2b(char n) const
{
	assert(!(0xc0 & n));
	return map[n];
}

char Base64Converter::Base64Map::b2n(char b) const
{
	assert(!(0x80 & b));
	return rmap[b];
}

Base64Converter::Base64Map const Base64Converter::map;

struct Base64Converter::Impl {
	template <class Iter>
	std::vector<char> octa2Hexa(Iter from, Iter end) const
	{
		std::vector<char> hexa;
		hexa.push_back(*from >> 2 & 0x3f);
		hexa.push_back(*from << 4 & 0x30);
		if (++from == end) return hexa;
		hexa.back() |= *from >> 4 & 0x0f;
		hexa.push_back(*from << 2 & 0x3c);
		if (++from == end) return hexa;
		hexa.back() |= *from >> 6 & 0x03;
		hexa.push_back(*from & 0x3f);
		return hexa;
	}
	std::vector<char> hexa2Octa(std::vector<char> const& hexa) const
	{
		std::vector<char> octa;
		auto it = hexa.begin();
		if (it == hexa.end()) return octa;
		octa.push_back(*it << 2 & 0xfc);
		if (++it == hexa.end()) return octa;
		octa.back() |= *it >> 4 & 0x03;
		octa.push_back(*it << 4 & 0xf0);
		if (++it == hexa.end()) return octa;
		octa.back() |= *it >> 2 & 0x0f;
		octa.push_back(*it << 6 & 0xc0);
		if (++it == hexa.end()) return octa;
		octa.back() |= *it & 0x3f;
		return octa;
	}
};

struct Base64Converter::FactoryRegister {
	FactoryRegister()
	{
		ReversibleFactory::registFactory("base64",
			[]() -> ReversibleBase * { return new Base64Converter(); });
	}
};

Base64Converter::FactoryRegister Base64Converter::factory_register;

Base64Converter::Base64Converter() :
	impl{new Base64Converter::Impl{}}
{}

Base64Converter::~Base64Converter() = default;

std::string Base64Converter::encode(std::string const& text) const
{
	std::string dist;
	for (int i = 0; i < text.size(); i += 3) {
		auto hexa = impl->octa2Hexa(text.begin() + i, text.end());
		for (int j = 0; j < 4; j++)
			dist.push_back(j < hexa.size() ? map.n2b(hexa[j]) : '=');
	}

	return dist;
}

std::string Base64Converter::decode(std::string const& text) const
{
	std::string dist;
	for (int i = 0; i < text.size(); i += 4) {
		std::vector<char> bits;
		for (int j = 0; j < 4; j++) {
			if (i + j >= text.size() || text[i + j] == '=') break;
			if (text[i + j] & 0x80)
				throw Base64ConverterException("It's 8bits charactor");
			char c = map.b2n(text[i + j]);
			if (c == static_cast<char>(-1))
				throw Base64ConverterException("It's not a charactor of BASE64");
			bits.push_back(c);
		}
		auto octa = impl->hexa2Octa(bits);
		std::move(octa.begin(), octa.end(), std::back_inserter(dist));
	}

	return dist;
}

}	//namespace apides
