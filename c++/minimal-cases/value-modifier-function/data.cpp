/**
	@file	data.cpp
	@brief	データオブジェクトクラス実装
*/

#include "data.h"
#include <unordered_map>

namespace minimum {

struct Data::Impl {
	std::vector<std::string> keys;
	std::unordered_map<std::string, decltype(keys)::size_type> key_order;
	std::vector<std::string> values;

	Data::UpdateCallBack callback;

	Impl();
	~Impl() = default;
};

static void nonOperationUpdating(Data::DetailReference const&)
{}

Data::Impl::Impl() :
	callback{nonOperationUpdating}
{}

Data::Data() :
	impl{new Data::Impl{}}
{}

Data::~Data() = default;

void Data::appendKey(std::string const& key, std::string const& value)
{
	impl->key_order[key] = impl->keys.size();
	impl->keys.push_back(key);
	impl->values.push_back(value);

	impl->callback(DetailReference{impl->keys, impl->values});
}

void Data::dropKey(std::string const& key)
{
	auto pos = impl->key_order[key];
	impl->key_order.erase(key);
	impl->keys.erase(impl->keys.begin() + pos);
	impl->values.erase(impl->values.begin() + pos);

	// キーオーダー振り直し
	for (auto i = pos; i < impl->keys.size(); i++)
		impl->key_order[impl->keys[i]] = i;
	
	impl->callback(DetailReference{impl->keys, impl->values});
}

std::string const& Data::operator[](std::string const& key) const
{
	return impl->values[impl->key_order[key]];
}

Data & Data::modify(std::function<void(Data::Detail &)> func)
{
	Detail detail{impl->keys, impl->values};
	func(detail);
	if (impl->keys == detail.keys && impl->values == detail.values) return *this;

	impl->keys = detail.keys;
	impl->values = detail.values;
	impl->key_order.clear();
	for (typename decltype(impl->keys)::size_type i = 0; i < impl->keys.size(); i++)
		impl->key_order[impl->keys[i]] = i;

	impl->callback(DetailReference{impl->keys, impl->values});

	return *this;
}

Data & Data::setUpdateCallBack(UpdateCallBack callback)
{
	impl->callback = callback;

	return *this;
}

}	//namespace minimum
