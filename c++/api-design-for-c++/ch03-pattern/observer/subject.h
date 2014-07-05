/**
	@file	subject.h
	@brief	サブジェクトオブジェクトの抽象クラス宣言
*/
#ifndef APIDES_SUBJECT_H
#define APIDES_SUBJECT_H

#include <memory>
#include <boost/noncopyable.hpp>
#include <exception>

namespace apides {

class ObserverBase;

class SubjectBase : private boost::noncopyable {
public:
	SubjectBase();
	virtual ~SubjectBase();
	void subscribe(int message, std::shared_ptr<ObserverBase> observer);
	void unsubscribe(int message, std::shared_ptr<ObserverBase> observer);

	class UnsubscribeException : public std::exception {
		std::string message;
	public:
		explicit UnsubscribeException(char const* msg) :
			std::exception{}, message{msg}
		{}
		virtual ~UnsubscribeException() = default;
		virtual char const* what() const override { return message.c_str(); }
	};

	class NotifyException : public std::exception {
		std::string message;
	public:
		explicit NotifyException(char const* msg) :
			std::exception{}, message{msg}
		{}
		virtual ~NotifyException() = default;
		virtual char const* what() const override { return message.c_str(); }
	};

protected:
	void notifyObserver(int message);

private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};

} //namespace apides

#endif	//APIDES_SUBJECT_H

