/**
	@file	datetime.h
	@brief	日時を扱うモデルデータのクラス宣言
*/

#ifndef APIDES_DATETIME_H
#define APIDES_DATETIME_H

#include <memory>
#include <string>
#include <boost/noncopyable.hpp>
#include <boost/datetime.hpp>

namespace apides {

class DateTime : private boost::noncopyable {
	struct Impl;
	std::unique_ptr<Impl> impl;
	using p_time = boost::posix_time;

public:
	explicit DateTime(p_time::ptime const& time = p_time::second_clock::local_time());

	std::string toString() const;

	void updateNow();
};

}	//namespace apides

#endif	//APIDES_DATETIME_H
