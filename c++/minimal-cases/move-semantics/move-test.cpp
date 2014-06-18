#include <iostream>
#include <utility>
#include <string>

class Hoge {
	std::string name;
public:
	Hoge() = default;
	explicit Hoge(std::string const& nm) : name{nm} {}
	Hoge(Hoge const& hoge) :
		name{hoge.name}
	{
		std::cout << "Hoge: <" << name << "> コピーコンストラクタが呼ばれました。" << std::endl;
	}
	Hoge(Hoge && hoge) :
		name{std::move(hoge.name)}
	{
		std::cout << "Hoge: <" << name << "> ムーブコンストラクタが呼ばれました。" << std::endl;
	}
	Hoge & operator=(Hoge const& hoge)
	{
		name = hoge.name;
		std::cout << "Hoge: <" << name << "> コピー代入演算子が呼ばれました。" << std::endl;
		return *this;
	}
	Hoge & operator=(Hoge && hoge)
	{
		name = std::move(hoge.name);
		std::cout << "Hoge: <" << name << "> ムーブ代入演算子が呼ばれました。" << std::endl;
		return *this;
	}
	~Hoge() = default;

	std::string const& getName() const { return name; }
	void setName(std::string const& nm) { name = nm; }
};

Hoge genHoge(std::string const& hoge_name)
{
	return Hoge(hoge_name);
}

int main()
{
	//auto hoge_foo = genHoge("foo");
	Hoge hoge_foo(genHoge("foo"));
	std::cout << "hoge_foo.name = " << hoge_foo.getName() << std::endl;

	hoge_foo.setName("bar");
	Hoge hoge_bar = hoge_foo;
	std::cout << "hoge_bar.name = " << hoge_bar.getName() << std::endl;

	hoge_foo.setName("baz");
	Hoge hoge_baz = std::move(hoge_foo);
	std::cout << "hoge_baz.name = " << hoge_baz.getName() << std::endl;

	return 0;
}

