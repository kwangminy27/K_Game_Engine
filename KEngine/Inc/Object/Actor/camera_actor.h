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

		void CreateView(Vector3 const& _eye, Vector3 const& _focus, Vector3 const& _up);
		void CreateProjection(float _width, float _height, float _near, float _far);
		void CreateProjection(float _fov_angle, float _width, float _height, float _near, float _far);

		Matrix const& view() const;
		Matrix const& projection() const;

	private:
		CameraActor() = default;
		CameraActor(CameraActor const& _other);
		CameraActor(CameraActor&& _other) noexcept;
		CameraActor& operator=(CameraActor const&) = delete;
		CameraActor& operator=(CameraActor&&) noexcept = default;

		virtual void _Finalize() override;
	};
}
