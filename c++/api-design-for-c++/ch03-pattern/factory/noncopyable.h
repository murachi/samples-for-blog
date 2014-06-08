/**
	@file	noncopyable.h
	@brief	コピー禁止クラス実装用の Mix-in

	boost::noncopyable を使えばいいんだけど、これだけのために boost 要求するのも癪なので…。
*/
#ifndef APIDES_FACTORY_NONCOPYABLE_H
#define APIDES_FACTORY_NONCOPYABLE_H

namespace apides {

class NonCopyable {
protected:
	NonCopyable() {}
	~NonCopyable() {}

private:
	NonCopyable(NonCopyable const&) = delete;
	NonCopyable & operator=(NonCopyable const&) = delete;
	NonCopyable(NonCopyable &&) = delete;
	NonCopyable & operator=(NonCopyable &&) = delete;
};

}	//namespace apides

#endif	//APIDES_FACTORY_NONCOPYABLE_H
