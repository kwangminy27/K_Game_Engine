#include "KEngine.h"
#include "level.h"

#include "layer.h"

std::shared_ptr<K::Layer> K::Level::layer_dummy_{};

void K::Level::Initialize()
{
	try
	{
		_CreateLayer("Default", 0);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Level::Initialize" << std::endl;
	}
}

std::shared_ptr<K::Layer> const& K::Level::FindLayer(TAG const& _tag) const
{
	auto iter = std::find_if(layer_list_.begin(), layer_list_.end(), [&_tag](std::shared_ptr<Layer> const& _p) {
		return _p->tag() == _tag;
	});

	if (iter == layer_list_.end())
		return layer_dummy_;

	return *iter;
}

K::Level::Level(Level const& _other) : Tag(_other)
{
	layer_list_ = _other.layer_list_;
}

K::Level::Level(Level&& _other) noexcept : Tag(std::move(_other))
{
	layer_list_ = std::move(_other.layer_list_);
}

void K::Level::_Finalize()
{
}

void K::Level::_Input(float _time)
{
	for (auto iter = layer_list_.begin(); iter != layer_list_.end();)
	{
		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			(*iter)->_Input(_time);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			iter = layer_list_.erase(iter);
			break;
		}
	}
}

void K::Level::_Update(float _time)
{
	for (auto iter = layer_list_.begin(); iter != layer_list_.end();)
	{
		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			(*iter)->_Update(_time);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			iter = layer_list_.erase(iter);
			break;
		}
	}
}

void K::Level::_Collision(float _time)
{
	for (auto iter = layer_list_.begin(); iter != layer_list_.end();)
	{
		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			(*iter)->_Collision(_time);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			iter = layer_list_.erase(iter);
			break;
		}
	}
}

void K::Level::_Render(float _time)
{
	for (auto iter = layer_list_.begin(); iter != layer_list_.end();)
	{
		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			(*iter)->_Render(_time);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			iter = layer_list_.erase(iter);
			break;
		}
	}
}

void K::Level::_CreateLayer(std::string const& _tag, uint32_t _order)
{
	auto layer = std::shared_ptr<Layer>{ new Layer, [](Layer* _p) {
		_p->_Finalize();
		delete _p;
	} };

	auto id = std::count_if(layer_list_.begin(), layer_list_.end(), [&_tag](std::shared_ptr<Layer> const& _p) {
		return _p->tag().first == _tag;
	});

	layer->set_tag(std::make_pair(_tag, static_cast<uint32_t>(id)));
	layer->set_order(_order);
	layer->set_level(shared_from_this());

	layer->Initialize();

	layer_list_.push_back(std::move(layer));

	layer_list_.sort([](std::shared_ptr<Layer> const& _p1, std::shared_ptr<Layer> const& _p2) {
		return _p1->order() < _p2->order();
	});
}
