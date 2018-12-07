#include "KEngine.h"
#include "renderer.h"

#include "Resource/mesh.h"
#include "Rendering/rendering_manager.h"
#include "Rendering/shader.h"
#include "Rendering/render_state.h"
#include "World/world_manager.h"
#include "Object/Actor/camera_actor.h"
#include "Object/Component/transform.h"
#include "Object/Component/camera.h"
#include "Object/Component/material.h"

void K::Renderer::Initialize()
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
		std::cout << "Renderer::Initialize" << std::endl;
	}
}

void K::Renderer::Render(float _time)
{
	auto const& material = owner()->FindComponent({ "Mateiral", 0 });

	_UpdateConstantBuffers(_time);

	for (auto const& render_state : render_state_vector_)
		render_state->SetToShader();

	shader_->SetToShader();

	for (auto i = 0; i < mesh_->GetContainerSize(); ++i)
	{
		for (auto j = 0; i < mesh_->GetSubsetSize(i); ++j)
		{
			CPTR_CAST<Material>(material)->SetToShader(i, j);

			mesh_->Render(i, j);
		}
	}
}

K::CPTR K::Renderer::Clone() const
{
	return CPTR{ new Renderer{ *this }, [](Component* _p) {
		static_cast<Renderer*>(_p)->_Finalize();
		delete static_cast<Renderer*>(_p);
	} };
}

void K::Renderer::Serialize(InputMemoryStream& _imstream)
{
}

void K::Renderer::Serialize(OutputMemoryStream& _omstream)
{
}

void K::Renderer::AddRenderState(std::shared_ptr<RenderState> const& _render_state)
{
	render_state_vector_.push_back(_render_state);
}

void K::Renderer::set_mesh(std::shared_ptr<Mesh> const& _mesh)
{
	mesh_ = _mesh;
}

void K::Renderer::set_shader(std::shared_ptr<Shader> const& _shader)
{
	shader_ = _shader;
}

K::Renderer::Renderer(Renderer const& _other) : Component(_other)
{
	mesh_ = _other.mesh_;
	shader_ = _other.shader_;
	render_state_vector_ = _other.render_state_vector_;
}

K::Renderer::Renderer(Renderer&& _other) noexcept : Component(std::move(_other))
{
	mesh_ = std::move(_other.mesh_);
	shader_ = std::move(_other.shader_);
	render_state_vector_ = std::move(_other.render_state_vector_);
}

void K::Renderer::_Finalize()
{
}

void K::Renderer::_UpdateConstantBuffers(float _time)
{
	auto const& transform = owner()->FindComponent({ "Transform", 0 });

	auto const& main_camera = WorldManager::singleton()->FindCamera({ "MainCamera", 0 });

	TransformConstantBuffer transform_CB{};
	transform_CB.world = CPTR_CAST<Transform>(transform)->world();
	transform_CB.view = main_camera->view();
	transform_CB.projection = main_camera->projection();
	transform_CB.WVP = transform_CB.world * transform_CB.view * transform_CB.projection;

	transform_CB.world = transform_CB.world.Transpose();
	transform_CB.view = transform_CB.view.Transpose();
	transform_CB.projection = transform_CB.projection.Transpose();
	transform_CB.WVP = transform_CB.WVP.Transpose();

	RenderingManager::singleton()->UpdateConstantBuffer("Transform", &transform_CB);
}
