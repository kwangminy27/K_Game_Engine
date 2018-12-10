#include "KEngine.h"
#include "collider.h"

#include "Resource/mesh.h"
#include "Rendering/rendering_manager.h"
#include "Rendering/shader.h"

void K::Collider::Render(float _time)
{
#ifdef _DEBUG
	if (collided_collider_list_.empty())
		color_ = DirectX::Colors::Green.v;
	else
		color_ = DirectX::Colors::Red.v;

	RenderingManager::singleton()->UpdateConstantBuffer(COLLIDER, &color_);

	shader_->SetToShader();
	mesh_->Render();
#endif
}

void K::Collider::AddCallback(std::function<void(Collider*, Collider*, float)> const& _callback, COLLISION_CALLBACK_TYPE _type)
{
	callback_list_array_.at(static_cast<int>(_type)).push_back(_callback);
}

K::COLLIDER_TYPE K::Collider::type() const
{
	return type_;
}

void K::Collider::set_group_tag(std::string const& _tag)
{
	group_tag_ = _tag;
}

K::Collider::Collider(Collider const& _other) : Component(_other)
{
	color_ = _other.color_;
	mesh_ = _other.mesh_;
	shader_ = _other.shader_;

	min_ = _other.min_;
	max_ = _other.max_;
	type_ = _other.type_;
	group_tag_ = _other.group_tag_;
	section_idx_list_ = _other.section_idx_list_;
	collided_collider_list_ = _other.collided_collider_list_;
	callback_list_array_ = _other.callback_list_array_;
}

K::Collider::Collider(Collider&& _other) noexcept : Component(std::move(_other))
{
	color_ = std::move(_other.color_);
	mesh_ = std::move(_other.mesh_);
	shader_ = std::move(_other.shader_);

	min_ = std::move(_other.min_);
	max_ = std::move(_other.max_);
	type_ = std::move(_other.type_);
	group_tag_ = std::move(_other.group_tag_);
	section_idx_list_ = std::move(_other.section_idx_list_);
	collided_collider_list_ = std::move(_other.collided_collider_list_);
	callback_list_array_ = std::move(_other.callback_list_array_);
}

void K::Collider::_Finalize()
{
	for (auto iter = collided_collider_list_.begin(); iter != collided_collider_list_.end(); ++iter)
	{
		(*iter)->_OnCollisionLeave(this, 0.f);
		_OnCollisionLeave(*iter, 0.f);

		(*iter)->_RemoveCollidedCollider(this);
	}
}

bool K::Collider::_CollisionPointToPoint(Vector3 const& _src, Vector3 const& _dest)
{
	return _src == _dest;
}

bool K::Collider::_CollisionCircleToPoint(Circle const& _src, Vector3 const& _dest)
{
	return (_dest - _src.center).Length() <= _src.radius;
}

bool K::Collider::_CollisionCircleToCircle(Circle const& _src, Circle const& _dest)
{
	return (_dest.center - _src.center).Length() <= _src.radius + _dest.radius;
}

bool K::Collider::_CollisionAABBToPoint(AABB const& _src, Vector3 const& _dest)
{
	return false;
}

bool K::Collider::_CollisionAABBToCircle(AABB const& _src, Circle const& _dest)
{
	return false;
}

bool K::Collider::_CollisionAABBToAABB(AABB const& _src, AABB const& _dest)
{
	return false;
}

bool K::Collider::_CollisionOOBBToPoint(OOBB const& _src, Vector3 const& _dest)
{
	return false;
}

bool K::Collider::_CollisionOOBBToCircle(OOBB const& _src, Circle const& _dest)
{
	return false;
}

bool K::Collider::_CollisionOOBBToAABB(OOBB const& _src, AABB const& _dest)
{
	return false;
}

bool K::Collider::_CollisionOOBBToOOBB(OOBB const& _src, OOBB const& _dest)
{
	return false;
}

void K::Collider::_OnCollisionEnter(Collider* _dest, float _time)
{
	for (auto const& callback : callback_list_array_.at(static_cast<int>(COLLISION_CALLBACK_TYPE::ENTER)))
		callback(this, _dest, _time);
}

void K::Collider::_OnCollisionStay(Collider* _dest, float _time)
{
	for (auto const& callback : callback_list_array_.at(static_cast<int>(COLLISION_CALLBACK_TYPE::STAY)))
		callback(this, _dest, _time);
}

void K::Collider::_OnCollisionLeave(Collider* _dest, float _time)
{
	for (auto const& callback : callback_list_array_.at(static_cast<int>(COLLISION_CALLBACK_TYPE::LEAVE)))
		callback(this, _dest, _time);
}

void K::Collider::_AddSectionIdx(uint32_t _idx)
{
	section_idx_list_.push_back(_idx);
}

void K::Collider::_UpdateCollidedCollider(float _time)
{
	for (auto iter = collided_collider_list_.begin(); iter != collided_collider_list_.end();)
	{
		if (group_tag_ != (*iter)->group_tag_)
		{
			(*iter)->_OnCollisionLeave(this, _time);
			_OnCollisionLeave(*iter, _time);
			
			(*iter)->_RemoveCollidedCollider(this);
			iter = collided_collider_list_.erase(iter);

			continue;
		}

		bool is_overlapped{};

		for (auto const& src_section_idx : section_idx_list_)
		{
			for (auto const& dest_section_idx : (*iter)->section_idx_list_)
			{
				if (src_section_idx == dest_section_idx)
					is_overlapped = true;
			}
		}

		if (false == is_overlapped)
		{
			(*iter)->_OnCollisionLeave(this, _time);
			_OnCollisionLeave(*iter, _time);

			(*iter)->_RemoveCollidedCollider(this);
			iter = collided_collider_list_.erase(iter);

			continue;
		}

		++iter;
	}
}

bool K::Collider::_IsCollidedCollider(Collider* _dest)
{
	auto iter = std::find(collided_collider_list_.begin(), collided_collider_list_.end(), _dest);

	if (iter == collided_collider_list_.end())
		return false;

	return true;
}

void K::Collider::_AddCollidedCollider(Collider* _dest)
{
	collided_collider_list_.push_back(_dest);
}

void K::Collider::_RemoveCollidedCollider(Collider* _dest)
{
	collided_collider_list_.remove(_dest);
}
