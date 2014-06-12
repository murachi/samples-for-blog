/**
	@file	base64.cpp
	@brief	BASE64変換処理を行うクラスの実装
*/

#include "base64.h"
#include <string>
#include <algorithm>
#include <vector>
#include <array>
#include <cassert>

namespace apides {

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

Base64Converter::~Base64Converter() = default;

std::string Base64Converter::encode(std::string const& text) const
{
	std::string dist;
	for (int i = 0; i < text.size(); i += 3) {
		std::vector<char> bits;
		bits.push_back(text[i] >> 2 & 0x3f);
		bits.push_back(text[i] << 4 & 0x30 |
			(i + 1 < text.size() ? text[i + 1] >> 4 & 0x0f : 0));
		if (i + 1 < text.size()) {
			bits.push_back(text[i + 1] << 2 & 0x3c |
				(i + 2 < text.size() ? text[i + 2] >> 6 & 0x03 : 0));
			if (i + 2 < text.size())
				bits.push_back(text[i + 2] & 0x3f);
		}
		std::array<char, 4> b64 = { map.n2b(bits[0]), map.n2b(bits[1]),
			bits.size() > 2 ? map.n2b(bits[2]) : '=',
			bits.size() > 3 ? map.n2b(bits[3]) : '=' };
		dist.append(b64.begin(), b64.end());
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
		std::array<char, 3> original = { 0, 0, 0 };
		original[0] |= bits.size() >= 1 ? bits[0] << 2 & 0xfc : 0;
		original[0] |= bits.size() >= 2 ? bits[1] >> 4 & 0x03 : 0;
		original[1] |= bits.size() >= 2 ? bits[1] << 4 & 0xf0 : 0;
		original[1] |= bits.size() >= 3 ? bits[2] >> 2 & 0x0f : 0;
		original[2] |= bits.size() >= 3 ? bits[2] << 6 & 0xc0 : 0;
		original[2] |= bits.size() >= 4 ? bits[3] & 0x3f : 0;
		dist.append(original.begin(), original.end());
	}

	return dist;
}

}	//namespace apides
