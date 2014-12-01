/**
	@file	share.cpp
	@brief	shared pointer を const にしても大丈夫かテスト
*/

#include <string>
#include <memory>
#include <iostream>

class Hoge {
	int hoge;
	std::string fuga;
public:
	void setHoge(int v) { hoge = v; }
	int getHoge() const { return hoge; }

	void setFuga(std::string const& v) { fuga = v; }
	std::string const& getFuga() const { return fuga; }

	Hoge() : hoge{0}, fuga{""} {}
	~Hoge() = default;
};

void sayHoge(std::shared_ptr<const Hoge> hoge)
{
	std::cout << "hoge = " << hoge->getHoge()
		<< ", fuga = " << hoge->getFuga() << std::endl;
}

int main()
{
	auto hoge = std::make_shared(Hoge{});
	hoge->setHoge(10);
	hoge->setFuga("こんにちわこんにちわ!!");

	syaHoge(hoge);

	return 0;
}

