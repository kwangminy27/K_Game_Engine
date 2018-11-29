#include "KEngine.h"
#include "layer.h"

#include "level.h"
#include "Object/actor.h"

K::APTR K::Layer::actor_dummy_{};

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

K::APTR const& K::Layer::FindActor(TAG const& _tag) const
{
	auto iter = std::find_if(actor_list_.begin(), actor_list_.end(), [&_tag](APTR const& _p) {
		return _p->tag() == _tag;
	});

	if (iter == actor_list_.end())
		return actor_dummy_;

	return *iter;
}

void K::Layer::AddActor(APTR const& _actor)
{
	actor_list_.push_back(std::move(_actor));
}

void K::Layer::RemoveActor(TAG const& _tag)
{
	if (auto const& actor = FindActor(_tag))
		actor_list_.remove(actor);
}

std::shared_ptr<K::Level> K::Layer::level() const
{
	return level_.lock();
}

uint32_t K::Layer::order() const
{
	return order_;
}

void K::Layer::set_level(std::shared_ptr<Level> const& _level)
{
	level_ = _level;
}

void K::Layer::set_order(uint32_t _order)
{
	order_ = _order;
}

K::Layer::Layer(Layer&& _other) noexcept : Tag(std::move(_other))
{
	level_ = std::move(_other.level_);
	order_ = std::move(_other.order_);
	actor_list_ = std::move(_other.actor_list_);
}

void K::Layer::_Finalize()
{
}

void K::Layer::_Input(float _time)
{
	for (auto iter = actor_list_.begin(); iter != actor_list_.end();)
	{
		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			(*iter)->Input(_time);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			iter = actor_list_.erase(iter);
			break;
		}
	}
}

void K::Layer::_Update(float _time)
{
	for (auto iter = actor_list_.begin(); iter != actor_list_.end();)
	{
		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			(*iter)->Update(_time);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			iter = actor_list_.erase(iter);
			break;
		}
	}
}

void K::Layer::_Collision(float _time)
{
	for (auto iter = actor_list_.begin(); iter != actor_list_.end();)
	{
		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			(*iter)->Collision(_time);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			iter = actor_list_.erase(iter);
			break;
		}
	}
}

void K::Layer::_Render(float _time)
{
	for (auto iter = actor_list_.begin(); iter != actor_list_.end();)
	{
		switch ((*iter)->tag_state())
		{
		case TAG_STATE::NORMAL:
			(*iter)->Render(_time);
			++iter;
			break;

		case TAG_STATE::DISABLED:
			++iter;
			break;

		case TAG_STATE::DEAD:
			iter = actor_list_.erase(iter);
			break;
		}
	}
}
