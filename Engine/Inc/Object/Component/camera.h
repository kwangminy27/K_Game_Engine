#pragma once

#include "component.h"

namespace K
{
	class K_ENGINE_DLL Camera final : public Component
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual CPTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

		void CreateView(Vector3 const& _eye, Vector3 const& _focus, Vector3 const& _up);
		void CreateProjection(float _width, float _height, float _near, float _far);
		void CreateProjection(float _fov_angle, float _width, float _height, float _near, float _far);

		Matrix const& view() const;
		Matrix const& projection() const;

	private:
		Camera() = default;
		Camera(Camera const& _other);
		Camera(Camera&& _other) noexcept;
		Camera& operator=(Camera const&) = delete;
		Camera& operator=(Camera&&) noexcept = default;

		virtual void _Finalize() override;

		Matrix view_{ Matrix::Identity };
		Matrix projection_{ Matrix::Identity };
	};
}
