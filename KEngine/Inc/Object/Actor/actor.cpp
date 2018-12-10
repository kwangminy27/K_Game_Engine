#include "KEngine.h"
#include "actor.h"

#include "World/layer.h"
#include "Object/Component/component.h"
#include "Object/Component/transform.h"

K::CPTR K::Actor::component_dummy_{};
K::APTR K::Actor::child_dummy_{};

void K::Actor::Initialize()
{
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

std::list<K::Component*> const& K::Actor::FindComponent(std::string const& _tag) const
{
	static std::list<Component*> component_list{};

	component_list.clear();

	for (auto const& component : component_list_)
	{
		if (component->tag().first == _tag)
			component_list.push_back(component.get());
	}

	return component_list;
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

void K::Actor::AddComponent(CPTR& _component)
{
	_component->set_owner(shared_from_this());

	component_list_.push_back(std::move(_component));
}

void K::Actor::AddChild(APTR& _child)
{
	_child->set_parent(shared_from_this());

	child_list_.push_back(std::move(_child));
}

void K::Actor::RemoveComponent(CPTR const& _component)
{
	component_list_.remove(_component);
}

void K::Actor::RemoveChild(APTR const& _child)
{
	_child->set_parent(Layer::actor_dummy_);

	child_list_.remove(_child);
}

std::shared_ptr<K::Level> K::Actor::level() const
{
	return level_.lock();
}

std::shared_ptr<K::Layer> K::Actor::layer() const
{
	return layer_.lock();
}

K::APTR K::Actor::parent() const
{
	return parent_.lock();
}

std::list<K::CPTR> const& K::Actor::component_list() const
{
	return component_list_;
}

std::list<K::APTR> const& K::Actor::child_list() const
{
	return child_list_;
}

void K::Actor::set_parent(APTR const& _actor)
{
	parent_ = _actor;
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
	parent_ = _other.parent_;

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
	parent_ = std::move(_other.parent_);
	component_list_ = std::move(_other.component_list_);
	child_list_ = std::move(_other.child_list_);
}

void K::Actor::_Finalize()
{
}

void K::Actor::_Input(float _time)
{
}

void K::Actor::_Update(float _time)
{
}

void K::Actor::_Collision(float _time)
{
}

void K::Actor::_Render(float _time)
{
}

void K::Actor::__Input(float _time)
{
	_Input(_time);

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

void K::Actor::__Update(float _time)
{
	_Update(_time);

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

	auto const& transform = static_cast<Transform*>(FindComponent(TAG{ TRANSFORM, 0 }).get());

	if (transform->dirty_flag())
	{
		for (auto const& child : child_list_)
		{
			auto const& child_transform = static_cast<Transform*>(child->FindComponent(TAG{ TRANSFORM, 0 }).get());

			child_transform->set_parent_scaling(transform->world_scaling());
			child_transform->set_parent_rotation(transform->world_rotation());
			child_transform->set_parent_translation(transform->world_translation());

			child_transform->set_dirty_flag(true);
		}

		transform->set_dirty_flag(false);
	}
}

void K::Actor::__Collision(float _time)
{
	_Collision(_time);

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

void K::Actor::__Render(float _time)
{
	_Render(_time);

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

K::ActorClient::ActorClient(ActorClient const& _other) : Actor(_other)
{
}

K::ActorClient::ActorClient(ActorClient&& _other) noexcept : Actor(std::move(_other))
{
}

K::ActorServer::ActorServer(ActorServer const& _other) : Actor(_other)
{
}

K::ActorServer::ActorServer(ActorServer&& _other) noexcept : Actor(std::move(_other))
{
}
