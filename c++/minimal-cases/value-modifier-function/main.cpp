/**
	@file	main.cpp
	@brief	メインルーチン
*/

#include "data.h"
#include <iostream>
#include <string>

struct A {
	void func(minimum::Data &data, std::string const& birth, std::string const& company)
	{
		data.modify([&birth, &company](minimum::Data::Detail &detail){
			detail.keys.erase(detail.keys.begin() + 1);
			auto role = detail.values[1];
			detail.values.erase(detail.values.begin() + 1);
			detail.keys.push_back("birth");
			// birth = "書き換えちゃった^-^";	//エラー
			detail.values.push_back(birth);
			detail.keys.push_back("company");
			detail.values.push_back(company);
		});
	}
};

int main()
{
	minimum::Data data{};
	data.setUpdateCallBack([](minimum::Data::DetailReference const& detail){
		for (decltype(detail.keys.size()) i = 0; i < detail.keys.size(); i++)
			std::cout << detail.keys[i] << " : " << detail.values[i] << std::endl;
		std::cout << std::endl;
	});

	data.appendKey("name", "村山 俊之");
	data.appendKey("role", "社長");
	data.appendKey("age", "36");

	data.dropKey("age");

	A a;
	a.func(data, "1978/2/7", "株式会社はらぺこ");

	return 0;
}
