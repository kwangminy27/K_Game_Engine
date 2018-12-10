#include "KEngine.h"
#include "rendering_manager.h"

#include "device_manager.h"
#include "shader.h"
#include "render_state.h"
#include "depth_stencil_state.h"
#include "blend_state.h"

std::shared_ptr<K::Shader> K::RenderingManager::shader_dummy_{};
std::shared_ptr<K::RenderState> K::RenderingManager::render_state_dummy_{};
std::shared_ptr<K::ConstantBuffer> K::RenderingManager::CB_dummy_{};

void K::RenderingManager::Initialize()
{
	try
	{
#pragma region Shader
		std::vector<CSO_DESC> cso_desc_vector{};
		cso_desc_vector.push_back({ SHADER_TYPE::VERTEX, L"BasicVS.cso" });
		cso_desc_vector.push_back({ SHADER_TYPE::PIXEL, L"BasicPS.cso" });

		std::vector<D3D11_INPUT_ELEMENT_DESC> input_element_desc_vector{};
		input_element_desc_vector.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		input_element_desc_vector.push_back({ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		_CreateShader(BASIC_SHADER, cso_desc_vector, input_element_desc_vector, SHADER_PATH);

		cso_desc_vector.clear();
		cso_desc_vector.push_back({ SHADER_TYPE::VERTEX, L"BasicTexVS.cso" });
		cso_desc_vector.push_back({ SHADER_TYPE::PIXEL, L"BasicTexPS.cso" });

		input_element_desc_vector.clear();
		input_element_desc_vector.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		input_element_desc_vector.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		_CreateShader(BASIC_TEX_SHADER, cso_desc_vector, input_element_desc_vector, SHADER_PATH);

		cso_desc_vector.clear();
		cso_desc_vector.push_back({ SHADER_TYPE::VERTEX, L"BasicAnimation2DVS.cso" });
		cso_desc_vector.push_back({ SHADER_TYPE::PIXEL, L"BasicTexPS.cso" });

		input_element_desc_vector.clear();
		input_element_desc_vector.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });
		input_element_desc_vector.push_back({ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		_CreateShader(BASIC_ANIMATION_2D_SHADER, cso_desc_vector, input_element_desc_vector, SHADER_PATH);

		cso_desc_vector.clear();
		cso_desc_vector.push_back({ SHADER_TYPE::VERTEX, L"ColliderVS.cso" });
		cso_desc_vector.push_back({ SHADER_TYPE::PIXEL, L"ColliderPS.cso" });

		input_element_desc_vector.clear();
		input_element_desc_vector.push_back({ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 });

		_CreateShader(COLLIDER_SHADER, cso_desc_vector, input_element_desc_vector, SHADER_PATH);
#pragma endregion

#pragma region RenderState
		D3D11_DEPTH_STENCILOP_DESC front_face{};
		front_face.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		front_face.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		front_face.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		front_face.StencilFunc = D3D11_COMPARISON_NEVER;

		D3D11_DEPTH_STENCILOP_DESC back_face{};
		back_face.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		back_face.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		back_face.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		back_face.StencilFunc = D3D11_COMPARISON_NEVER;

		_CreateDepthStencilState(
			DEPTH_DISABLE,
			false, D3D11_DEPTH_WRITE_MASK_ZERO, D3D11_COMPARISON_NEVER,
			false, D3D11_DEFAULT_STENCIL_READ_MASK, D3D11_DEFAULT_STENCIL_WRITE_MASK, front_face, back_face
		);

		std::vector<D3D11_RENDER_TARGET_BLEND_DESC> render_target_blend_desc_vector{};

		D3D11_RENDER_TARGET_BLEND_DESC rtbd{};
		rtbd.BlendEnable = true;
		rtbd.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		rtbd.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		rtbd.BlendOp = D3D11_BLEND_OP_ADD;
		rtbd.SrcBlendAlpha = D3D11_BLEND_ONE;
		rtbd.DestBlendAlpha = D3D11_BLEND_ZERO;
		rtbd.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		rtbd.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		render_target_blend_desc_vector.push_back(std::move(rtbd));

		_CreateBlendState(ALPHA_BLEND, false, false, render_target_blend_desc_vector);
#pragma endregion

#pragma region ConstantBuffer
		_CreateConstantBuffer(TRANSFORM, sizeof(TransformConstantBuffer), static_cast<uint8_t>(SHADER_TYPE::VERTEX) | static_cast<uint8_t>(SHADER_TYPE::PIXEL), 0);
		_CreateConstantBuffer(MATERIAL, sizeof(MaterialConstantBuffer), static_cast<uint8_t>(SHADER_TYPE::VERTEX) | static_cast<uint8_t>(SHADER_TYPE::PIXEL), 1);
		_CreateConstantBuffer(ANIMATION_2D, sizeof(Animation2DConstantBuffer), static_cast<uint8_t>(SHADER_TYPE::VERTEX) | static_cast<uint8_t>(SHADER_TYPE::PIXEL), 2);
		_CreateConstantBuffer(COLLIDER, sizeof(Vector4), static_cast<uint8_t>(SHADER_TYPE::VERTEX) | static_cast<uint8_t>(SHADER_TYPE::PIXEL), 3);
#pragma endregion
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

std::shared_ptr<K::RenderState> const& K::RenderingManager::FindRenderState(std::string const& _tag) const
{
	auto iter = render_state_map_.find(_tag);

	if (iter == render_state_map_.end())
		return render_state_dummy_;

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

	memcpy_s(ms.pData, CB->size, _data, CB->size);

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
	auto render_state = std::shared_ptr<RenderState>{ new DepthStencilState, [](DepthStencilState* _p) {
		delete _p;
	} };

	std::static_pointer_cast<DepthStencilState>(render_state)->_CreateState(
		_depth_enable,
		_depth_write_mask,
		_depth_func,
		_stencil_enable,
		_stencil_read_mask,
		_stencil_write_mask,
		_front_face,
		_back_face
	);

	render_state_map_.insert(std::make_pair(_tag, std::move(render_state)));
}

void K::RenderingManager::_CreateBlendState(
	std::string const& _tag,
	bool _alpha_to_coverage_enable,
	bool _independent_blend_enable,
	std::vector<D3D11_RENDER_TARGET_BLEND_DESC> const& _render_target_blend_desc_vector)
{
	auto render_state = std::shared_ptr<RenderState>{ new BlendState, [](BlendState* _p) {
		delete _p;
	} };

	std::static_pointer_cast<BlendState>(render_state)->_CreateState(
		_alpha_to_coverage_enable,
		_independent_blend_enable,
		_render_target_blend_desc_vector
	);

	render_state_map_.insert(std::make_pair(_tag, std::move(render_state)));
}

void K::RenderingManager::_CreateConstantBuffer(std::string const& _tag, uint32_t _size, uint8_t _shader_flag, uint32_t _slot)
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
