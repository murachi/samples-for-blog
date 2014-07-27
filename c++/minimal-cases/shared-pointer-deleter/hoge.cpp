#include <memory>
#include <vector>
#include <iostream>

class Hoge {
	int number;
public:
	explicit Hoge(int num) : number{num}
	{ std::cout << "Hoge " << number << " constructed." << std::endl; }
	~Hoge() { std::cout << "Hoge " << number << " destructed." << std::endl; }
};

int main()
{
	std::vector<std::shared_ptr<Hoge>> hogehoge;
	hogehoge.push_back(std::shared_ptr<Hoge>{new Hoge(1)});	// まぁ普通…
	Hoge fuga{2};
	hogehoge.push_back(std::shared_ptr<Hoge>{&fuga, [](Hoge *){}});	// を、行けるやん!!
	Hoge piyo{3};
	hogehoge.push_back(std::shared_ptr<Hoge>{&piyo});	//(アカン)

	return 0;
}
