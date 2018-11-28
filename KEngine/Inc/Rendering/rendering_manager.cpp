#include "KEngine.h"
#include "rendering_manager.h"

#include "device_manager.h"
#include "shader.h"
#include "state.h"
#include "depth_stencil_state.h"
#include "blend_state.h"

std::shared_ptr<K::Shader> K::RenderingManager::shader_dummy_{};
std::shared_ptr<K::State> K::RenderingManager::state_dummy_{};
std::shared_ptr<K::ConstantBuffer> K::RenderingManager::CB_dummy_{};

void K::RenderingManager::Initialize()
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
		std::cout << "RenderingManager::Initialize" << std::endl;
	}
}

std::shared_ptr<K::Shader> const& K::RenderingManager::FindShader(std::string const& _tag) const
{
	auto iter = shader_map_.find(_tag);

	if (iter == shader_map_.end())
		return shader_dummy_;

	return iter->second;
}

std::shared_ptr<K::State> const& K::RenderingManager::FindState(std::string const& _tag) const
{
	auto iter = state_map_.find(_tag);

	if (iter == state_map_.end())
		return state_dummy_;

	return iter->second;
}

std::shared_ptr<K::ConstantBuffer> const& K::RenderingManager::FindConstantBuffer(std::string const& _tag) const
{
	auto iter = CB_map_.find(_tag);

	if (iter == CB_map_.end())
		return CB_dummy_;

	return iter->second;
}

void K::RenderingManager::UpdateConstantBuffer(std::string const& _tag, void* _data)
{
	auto const& context = DeviceManager::singleton()->context();
	auto const& CB = FindConstantBuffer(_tag);

	if (!CB)
		throw std::exception{ "RenderingManager::UpdateConstantBuffer" };

	D3D11_MAPPED_SUBRESOURCE ms{};
	ThrowIfFailed(context->Map(CB->buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, NULL, &ms));

	memcpy_s(&ms, CB->size, _data, CB->size);

	context->Unmap(CB->buffer.Get(), 0);

	if (CB->shader_flag & static_cast<int>(SHADER_TYPE::VERTEX))
		context->VSSetConstantBuffers(CB->slot, 1, CB->buffer.GetAddressOf());

	if (CB->shader_flag & static_cast<int>(SHADER_TYPE::PIXEL))
		context->PSSetConstantBuffers(CB->slot, 1, CB->buffer.GetAddressOf());
}

void K::RenderingManager::_Finalize()
{
}

void K::RenderingManager::_CreateShader(
	std::string const& _tag,
	std::vector<CSO_DESC> const& _cso_desc_vector,
	std::vector<D3D11_INPUT_ELEMENT_DESC> const& _input_element_desc_vector,
	std::string const& _path_tag)
{
	if (FindShader(_tag))
		throw std::exception{ "RenderingManager::_CreateShader" };

	auto shader = std::shared_ptr<Shader>{ new Shader, [](Shader* _p) {
		delete _p;
	} };

	shader->_CreateShader(
		_cso_desc_vector,
		_input_element_desc_vector,
		_path_tag
	);

	shader_map_.insert(make_pair(_tag, std::move(shader)));
}

void K::RenderingManager::_CreateDepthStencilState(
	std::string const& _tag,
	BOOL _depth_enable,
	D3D11_DEPTH_WRITE_MASK _depth_write_mask,
	D3D11_COMPARISON_FUNC _depth_func,
	BOOL _stencil_enable,
	UINT8 _stencil_read_mask,
	UINT8 _stencil_write_mask,
	D3D11_DEPTH_STENCILOP_DESC const& _front_face,
	D3D11_DEPTH_STENCILOP_DESC const& _back_face)
{
	auto state = std::shared_ptr<State>{ new DepthStencilState, [](DepthStencilState* _p) {
		delete _p;
	} };

	std::static_pointer_cast<DepthStencilState>(state)->_CreateState(
		_depth_enable,
		_depth_write_mask,
		_depth_func,
		_stencil_enable,
		_stencil_read_mask,
		_stencil_write_mask,
		_front_face,
		_back_face
	);

	state_map_.insert(std::make_pair(_tag, std::move(state)));
}

void K::RenderingManager::_CreateBlendState(
	std::string const& _tag,
	bool _alpha_to_coverage_enable,
	bool _independent_blend_enable,
	std::vector<D3D11_RENDER_TARGET_BLEND_DESC> const& _render_target_blend_desc_vector)
{
	auto state = std::shared_ptr<State>{ new BlendState, [](BlendState* _p) {
		delete _p;
	} };

	std::static_pointer_cast<BlendState>(state)->_CreateState(
		_alpha_to_coverage_enable,
		_independent_blend_enable,
		_render_target_blend_desc_vector
	);

	state_map_.insert(std::make_pair(_tag, std::move(state)));
}

void K::RenderingManager::_CreateConstantBuffer(std::string const& _tag, uint32_t _size, uint32_t _shader_flag, uint32_t _slot)
{
	auto const& device = DeviceManager::singleton()->device();

	if (FindConstantBuffer(_tag))
		throw std::exception{ "RenderingManager::_CreateConstantBuffer" };

	auto CB = std::shared_ptr<ConstantBuffer>{ new ConstantBuffer, [](ConstantBuffer* _p) {
		delete _p;
	} };

	CB->size = _size;
	CB->shader_flag = _shader_flag;
	CB->slot = _slot;

	D3D11_BUFFER_DESC dbd{};
	dbd.ByteWidth = _size;
	dbd.Usage = D3D11_USAGE_DYNAMIC;
	dbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	dbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ThrowIfFailed(device->CreateBuffer(&dbd, nullptr, &CB->buffer));

	CB_map_.insert(std::make_pair(_tag, std::move(CB)));
}
