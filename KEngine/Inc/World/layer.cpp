#include "KEngine.h"
#include "layer.h"

#include "level.h"

void K::Layer::Initialize()
{
	try
	{
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Layer::Initialize" << std::endl;
	}
}

uint32_t K::Layer::order() const
{
	return order_;
}

std::shared_ptr<K::Level> K::Layer::level() const
{
	return level_.lock();
}

void K::Layer::set_order(uint32_t _order)
{
	order_ = _order;
}

void K::Layer::set_level(std::shared_ptr<Level> const& _level)
{
	level_ = _level;
}

K::Layer::Layer(Layer const& _other) : Tag(_other)
{
	order_ = _other.order_;
	level_ = _other.level_;
}

K::Layer::Layer(Layer&& _other) noexcept : Tag(std::move(_other))
{
	order_ = std::move(_other.order_);
	level_ = std::move(_other.level_);
}

void K::Layer::_Finalize()
{
}

void K::Layer::_Input(float _time)
{
}

void K::Layer::_Update(float _time)
{
}

void K::Layer::_Collision(float _time)
{
}

void K::Layer::_Render(float _time)
{
}
