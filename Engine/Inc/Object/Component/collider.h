#pragma once

#include "component.h"

namespace K
{
	enum class COLLIDER_TYPE
	{
		POINT,
		CIRCLE,
		AABB,
		OOBB,
		PIXEL
	};

	enum class COLLISION_CALLBACK_TYPE
	{
		ENTER,
		STAY,
		LEAVE,
		SIZE
	};

	struct Circle
	{
		Vector3 center;
		float radius;
	};

	struct AABB
	{
		Vector3 center;
		Vector3 extent;
	};

	struct OOBB
	{
		Vector3 center;
		Vector3 extent;
		Matrix rotation;
	};

	class K_ENGINE_DLL Collider : public Component
	{
		friend class CollisionManager;
	public:
		virtual void Initialize() override;

		virtual void Render(float _time);

		virtual CPTR Clone() const = 0;

		virtual void Serialize(InputMemoryStream& _imstream)  = 0;
		virtual void Serialize(OutputMemoryStream& _omstream)  = 0;

		void AddCallback(std::function<void(Collider*, Collider*, float)> const& _callback, COLLISION_CALLBACK_TYPE _type);

	protected:
		Collider() = default;
		Collider(Collider const& _other);
		Collider(Collider&& _other) noexcept;
		Collider& operator=(Collider const&) = delete;
		Collider& operator=(Collider&&) noexcept = default;
		virtual ~Collider() = default;

		virtual void _Finalize() override;

		virtual bool _Collision(Collider* _dest, float _time) = 0;

		void _OnCollisionEnter(Collider* _dest, float _time);
		void _OnCollisionStay(Collider* _dest, float _time);
		void _OnCollisionLeave(Collider* _dest, float _time);

		bool _CollisionOOBBToOOBB(OOBB const& _src, OOBB const& _dest);
		bool _CollisionOOBBToAABB(OOBB const& _src, AABB const& _dest);
		bool _CollisionOOBBToCircle(OOBB const& _src, Circle const& _dest);
		bool _CollisionOOBBToPoint(OOBB const& _src, Vector3 const& _dest);

		bool _CollisionAABBToAABB(AABB const& _src, AABB const& _dest);
		bool _CollisionAABBToCircle(AABB const& _src, Circle const& _dest);
		bool _CollisionAABBToPoint(AABB const& _src, Vector3 const& _dest);

		bool _CollisionCircleToCircle(Circle const& _src, Circle const& _dest);
		bool _CollisionCircleToPoint(Circle const& _src, Vector3 const& _dest);

		bool _CollisionPointToPoint(Vector3 const& _src, Vector3 const& _dest);

		void _AddSectionIdx(uint32_t _idx);

		void _UpdateCollidedCollider(float _time);

		bool _IsCollidedCollider(Collider* _dest);
		void _AddCollidedCollider(Collider* _dest);
		void _RemoveCollidedCollider(Collider* _dest);

		Vector3 min_{};
		Vector3 max_{};
		COLLIDER_TYPE type_{};
		std::string group_tag_{};
		std::list<uint32_t> section_idx_list_{};
		std::list<Collider*> collided_collider_list_{};
		std::array<std::list<std::function<void(Collider*, Collider*, float)>>, static_cast<int>(COLLISION_CALLBACK_TYPE::SIZE)> callback_list_array_{};
	};
}
