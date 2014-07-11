/**
	@file	fizzbuzz.cpp
	@brief	Fizz Buzz を C++11 で書いてみる
	@author	T.MURACHI (Toshiyuki Murayama)
	@date	2014/7/11
*/

#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>

class FizzBuzz {
	int number;
	
public:
	FizzBuzz() : number{1} {}
	~FizzBuzz() = default;
	
	FizzBuzz & operator++() { ++number; return *this; }
	FizzBuzz operator++(int) { auto old = *this; ++number; return old; }
	
	std::string get() const
	{
		return number % 15 ? number % 5 ? number % 3 ? boost::lexical_cast<std::string>(number) : "Fizz" : "Buzz" : "FizzBuzz";
	}
};

int main()
{
	FizzBuzz fb;
	for (int i = 0; i < 100; i++)
		std::cout << (fb++).get() << std::endl;
	
	return 0;
}
