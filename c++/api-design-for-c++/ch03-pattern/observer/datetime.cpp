/**
	@file	datetime.cpp
	@brief	日時を扱うモデルデータのクラス実装
*/

#include "datetime.h"

#include <iostream>

namespace apides {

struct DateTime::Impl {
	using p_time = boost::posix_time;
	p_time::ptime time;
	std::stringstream ss;

	explicit Impl(p_time::ptime const& tm) :
		time{tm}, ss{}
	{
		auto facet = new p_time::time_facet("%Y/%m/%d %H:%M:%S");
		ss.imbue(std::locale(std::cout.getloc(), facet));
	}
};

DateTime::DateTime(boost::posix_time::ptime const& time) :
	impl{new DateTime::Impl{time}}
{}

std::string DateTime::toString() const
{
	impl->ss.str("");
	impl->ss << impl->time;
	return impl->ss.str();
}

void DateTime::updateNow()
{
	impl->time = boost::posix_time::second_clock::local_time();
}

}	//namespace apides
