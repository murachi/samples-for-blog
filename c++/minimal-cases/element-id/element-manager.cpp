/**
	@file	element-manager.cpp
	@brief	要素 ID をプールし管理するクラス実装
*/

#include "element-manager.h"
#include "element.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <random>
#include <mutex>

#include <boost/lexical_cast.hpp>

namespace minimum {

/**
	@brief	ElementManager::ManagementException クラスの実装オブジェクトクラス
*/
struct ElementManager::ManagementException::Impl {
	std::string message;

	explicit Impl(std::string const& msg) : message{msg} {}
};

ElementManager::ManagementException::ManagementException(std::string const& msg) :
	impl{new ElementManager::ManagementException::Impl{msg}}
{}

ElementManager::ManagementException::~ManagementException() = default;

char const* ElementManager::ManagementException::what() const
{
	return impl->message.c_str();
}

/**
	@brief	ElementManager クラスの実装オブジェクトクラス
*/
struct ElementManager::Impl {
	std::unordered_map<std::string, std::shared_ptr<Element>> element_map;
	std::mt19937 random;
	std::mutex map_mutex;

	Impl() : element_map{}, random{std::random_device{}()} {}

	/**
		@brief	衝突しない ID に修正する
		@param[in]	elem_id		希望する要素 ID
		@return	衝突しない ID
	*/
	std::string checkIdCollisoin(std::string elem_id) const;
};

std::string ElementManager::Impl::checkIdCollision(std::string elem_id)
{
	if (impl->element_map.find(elem_id) == impl->element_map.end()) return elem_id;

	// ID の衝突を回避する
	auto id_src = elem_id;
	int i = 2;
	elem_id = id_src + "-" + boost::lexical_cast<string>(i);
	while (impl->element_map.find(elem_id) != impl->element_map.end()) {
		elem_id = id_src + "-" + boost::lexical_cast<string>(++i);
	}

	return elem_id;
}

ElementManager::ElementManager() :
	impl{new ElementManager::Impl{}}
{}

ElementManager::~ElementManager() = default;

std::string ElementManager::registElement(std::shared_ptr<Element> elem, std::string elem_id)
{
	if (elem_id.empty()) {
		// ID を自動生成する
		elem_id = std::string{"ID-"} + boost::lexical_cast<string>(random());
	}

	std::lock_guard<std::mutex> lock{impl->map_mutex};
	elem_id = impl->checkIdCollision(elem_id);
	impl->element_map[elem_id] = elem;

	return elem_id;
}

std::shared_ptr<Element> ElementManager::getElement(std::string const& elem_id) const
{
	if (elem_id.empty()) throw ManagementException{"Empty ID for query"};

	std::lock_guard<std::mutex> lock{impl->map_mutex};
	return impl->element_map.find(elem_id) == impl->element_map.end() ? std::shared_ptr<Element>{}
		: impl->element_map[elem_id];
}

std::string ElementManager::modifyElementId(std::string const& curr_id, std::string after_id)
{
	if (after_id.empty()) throw ManagementException{"Empty ID for modify"};

	std::lock_guard<std::mutex> lock{impl->map_mutex};
	if (impl->element_map.find(curr_id) == impl->element_map.end())
		throw ManagementException{"Element what is modified ID not found"};

	auto element = impl->element_map[curr_id];
	impl->element_map.erase(curr_id);

	after_id = impl->checkIdCollision(after_id);
	impl->element_map[after_id] = element;

	return after_id;
}

}	//namespace minimum
