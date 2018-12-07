#pragma once

#include "actor.h"

namespace K
{
	class K_ENGINE_DLL CameraActor final : public Actor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

		Matrix const& view() const;
		Matrix const& projection() const;

	private:
		CameraActor() = default;
		CameraActor(CameraActor const& _other);
		CameraActor(CameraActor&& _other) noexcept;
		CameraActor& operator=(CameraActor const&) = delete;
		CameraActor& operator=(CameraActor&&) noexcept = default;

		virtual void _Finalize() override;

		virtual void _Input(float _time);
		virtual void _Update(float _time);
		virtual void _Collision(float _time);
		virtual void _Render(float _time);
	};
}
