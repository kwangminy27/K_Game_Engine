#include "KEngine.h"
#include "actor.h"

#include "component.h"

K::CPTR K::Actor::component_dummy_{};
K::APTR K::Actor::child_dummy_{};

void K::Actor::Initialize()
{
}

void K::Actor::Input(float _time)
{
	for (auto iter = component_list_.begin(); iter != component_list_.end();)
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
			iter = component_list_.erase(iter);
			break;
		}
	}
}

void K::Actor::Update(float _time)
{
	for (auto iter = component_list_.begin(); iter != component_list_.end();)
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
			iter = component_list_.erase(iter);
			break;
		}
	}
}

void K::Actor::Collision(float _time)
{
	for (auto iter = component_list_.begin(); iter != component_list_.end();)
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
			iter = component_list_.erase(iter);
			break;
		}
	}
}

void K::Actor::Render(float _time)
{
	for (auto iter = component_list_.begin(); iter != component_list_.end();)
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
			iter = component_list_.erase(iter);
			break;
		}
	}
}

K::CPTR const& K::Actor::FindComponent(TAG const& _tag) const
{
	auto iter = std::find_if(component_list_.begin(), component_list_.end(), [&_tag](CPTR const& _p) {
		return _p->tag() == _tag;
	});

	if (iter == component_list_.end())
		return component_dummy_;

	return *iter;
}

K::APTR const& K::Actor::FindChild(TAG const& _tag) const
{
	auto iter = std::find_if(child_list_.begin(), child_list_.end(), [&_tag](APTR const& _p) {
		return _p->tag() == _tag;
	});

	if (iter == child_list_.end())
		return child_dummy_;

	return *iter;
}

void K::Actor::AddComponent(CPTR _component)
{
	component_list_.push_back(std::move(_component));
}

void K::Actor::AddChild(APTR const& _child)
{
	child_list_.push_back(std::move(_child));
}

void K::Actor::RemoveComponent(TAG const& _tag)
{
	if(auto const& component = FindComponent(_tag))
		component_list_.remove(component);
}

void K::Actor::RemoveChild(TAG const& _tag)
{
	if (auto const& child = FindChild(_tag))
		child_list_.remove(child);
}

std::shared_ptr<K::Level> K::Actor::level() const
{
	return level_.lock();
}

std::shared_ptr<K::Layer> K::Actor::layer() const
{
	return layer_.lock();
}

void K::Actor::set_level(std::shared_ptr<Level> const& _level)
{
	level_ = _level;
}

void K::Actor::set_layer(std::shared_ptr<Layer> const& _layer)
{
	layer_ = _layer;
}

K::Actor::Actor(Actor const& _other) : Tag(_other)
{
	level_ = _other.level_;
	layer_ = _other.layer_;

	component_list_.clear();
	for (auto const& component : _other.component_list_)
		component_list_.push_back(component->Clone());

	child_list_.clear();
	for (auto const& child : _other.child_list_)
		child_list_.push_back(child->Clone());
}

K::Actor::Actor(Actor&& _other) noexcept : Tag(std::move(_other))
{
	level_ = std::move(_other.level_);
	layer_ = std::move(_other.layer_);
	component_list_ = std::move(_other.component_list_);
	child_list_ = std::move(_other.child_list_);
}

void K::Actor::_Finalize()
{
}
