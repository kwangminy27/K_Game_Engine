#include "KEngine.h"
#include "collision_manager.h"

#include "Object/Actor/actor.h"

std::unique_ptr<K::CollisionGroup> K::CollisionManager::collision_group_dummy_{};

void K::CollisionManager::Initialize()
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
		std::cout << "CollisionManager::Initialize" << std::endl;
	}
}

void K::CollisionManager::Collision(float _time)
{
}

std::unique_ptr<K::CollisionGroup> const& K::CollisionManager::FindCollisionGroup(std::string const& _tag) const
{
	auto iter = collision_group_map_.find(_tag);

	if (iter == collision_group_map_.end())
		return collision_group_dummy_;

	return iter->second;
}

void K::CollisionManager::AddCollider(APTR const& _actor)
{
}

void K::CollisionManager::_Finalize()
{
}

void K::CollisionManager::_CreateCollisionGroup(std::string const& _tag, Vector3 const& _min, Vector3 const& _max, uint8_t _x_count, uint8_t _y_count, uint8_t _z_count)
{
	if (FindCollisionGroup(_tag))
		throw std::exception{ "CollisionManager::_CreateCollisionGroup" };

	auto collision_group = std::make_unique<CollisionGroup>();
	collision_group->min = _min;
	collision_group->max = _max;
	collision_group->x_count = _x_count;
	collision_group->y_count = _y_count;
	collision_group->z_count = _z_count;
	collision_group->total_count = _x_count * _y_count * _z_count;
	collision_group->section_diagonal_length = (_max - _min) / Vector3{ static_cast<float>(_x_count), static_cast<float>(_y_count), static_cast<float>(_z_count) };

	collision_group->section = std::unique_ptr<CollisionSection[], std::function<void(CollisionSection*)>>{ new CollisionSection[collision_group->total_count], [](CollisionSection* _p) {
		delete[] _p;
	} };

	for (int i = 0; i < collision_group->total_count; ++i)
		collision_group->section[i].reserve(100);

	collision_group_map_.insert(std::make_pair(_tag, std::move(collision_group)));
}
