#include "KEngine.h"
#include "resource_manager.h"

#include "path_manager.h"
#include "mesh.h"
#include "texture.h"
#include "sampler.h"

std::shared_ptr<K::Mesh> K::ResourceManager::mesh_dummy_{};
std::shared_ptr<K::Texture> K::ResourceManager::texture_dummy_{};
std::shared_ptr<K::Sampler> K::ResourceManager::sampler_dummy_{};
std::shared_ptr<K::ANIMATION_2D_CLIP_DESC> K::ResourceManager::animation_2d_clip_dummy_{};

void K::ResourceManager::Initialize()
{
	try
	{
#pragma region Mesh
		VertexTex tex_rect_vertices[4]{
			{ { -0.5f, -0.5f, 0.f }, { 0.f, 1.f } },
			{ { -0.5f, 0.5f, 0.f }, { 0.f, 0.f } },
			{ { 0.5f, 0.5f, 0.f }, { 1.f, 1.f } },
			{ { 0.5f, -0.5f, 0.f }, { 1.f, 0.f } }
		};

		uint16_t tex_rect_indices[6]{ 0, 1, 2, 1, 3, 2 };

		_CreateMesh(
			TEX_RECT, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			tex_rect_vertices, sizeof(VertexTex), 4, D3D11_USAGE_DEFAULT,
			tex_rect_indices, sizeof(uint16_t), 6, D3D11_USAGE_DEFAULT, DXGI_FORMAT_R16_UINT
		);

		Vector3 collider_rect_vertices[5]{
			{ -0.5f, -0.5f, 0.f },
			{ -0.5f, 0.5f, 0.f },
			{ 0.5f, 0.5f, 0.f },
			{ 0.5f, -0.5f, 0.f },
			{ -0.5f, -0.5f, 0.f }
		};

		_CreateMesh(
			COLLIDER_RECT, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
			collider_rect_vertices, sizeof(Vector3), 5, D3D11_USAGE_DEFAULT
		);

		Vector3 collider_circle_vertices[37]{};
		for (auto i = 0; i < 37; ++i)
			collider_circle_vertices[i] = { cos(DirectX::XMConvertToRadians(i * 10.f)) * 0.5f, sin(DirectX::XMConvertToRadians(i * 10.f)) * 0.5f, 0.f };

		_CreateMesh(
			COLLIDER_CIRCLE, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			collider_circle_vertices, sizeof(Vector3), 37, D3D11_USAGE_DEFAULT
		);
#pragma endregion

#pragma region Texture
#pragma endregion

#pragma region Sampler
		_CreateSampler(
			LINEAR_SAMPLER,
			D3D11_FILTER_MIN_MAG_MIP_LINEAR,
			D3D11_TEXTURE_ADDRESS_CLAMP,
			D3D11_TEXTURE_ADDRESS_CLAMP,
			D3D11_TEXTURE_ADDRESS_CLAMP
		);
#pragma endregion

#pragma region Animation2DClip
#pragma endregion
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "ResourceManager::Initialize" << std::endl;
	}
}

std::shared_ptr<K::Mesh> const& K::ResourceManager::FindMesh(std::string const& _tag) const
{
	auto iter = mesh_map_.find(_tag);

	if (iter == mesh_map_.end())
		return mesh_dummy_;

	return iter->second;
}

std::shared_ptr<K::Texture> const& K::ResourceManager::FindTexture(std::string const& _tag) const
{
	auto iter = texture_map_.find(_tag);

	if (iter == texture_map_.end())
		return texture_dummy_;

	return iter->second;
}

std::shared_ptr<K::Sampler> const& K::ResourceManager::FindSampler(std::string const& _tag) const
{
	auto iter = sampler_map_.find(_tag);

	if (iter == sampler_map_.end())
		return sampler_dummy_;

	return iter->second;
}

std::shared_ptr<K::ANIMATION_2D_CLIP_DESC> const& K::ResourceManager::FindAnimation2DClip(std::string const& _tag) const
{
	auto iter = animation_2d_clip_map_.find(_tag);

	if (iter == animation_2d_clip_map_.end())
		return animation_2d_clip_dummy_;

	return iter->second;
}

void K::ResourceManager::_Finalize()
{
}

void K::ResourceManager::_CreateMesh(
	std::string const& _tag, D3D11_PRIMITIVE_TOPOLOGY _topology,
	void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage)
{
	if (FindMesh(_tag))
		throw std::exception{ "ResourceManager::_CreateMesh" };

	auto mesh = std::shared_ptr<Mesh>{ new Mesh, [](Mesh* _p) {
		delete _p;
	} };

	mesh->_CreateMesh(_topology, _vtx_data, _vtx_stride, _vtx_count, _vtx_usage);

	mesh_map_.insert(std::make_pair(_tag, std::move(mesh)));
}

void K::ResourceManager::_CreateMesh(
	std::string const& _tag, D3D11_PRIMITIVE_TOPOLOGY _topology,
	void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage,
	void* _idx_data, int _idx_stride, int _idx_count, D3D11_USAGE _idx_usage, DXGI_FORMAT _idx_format)
{
	if (FindMesh(_tag))
		throw std::exception{ "ResourceManager::_CreateMesh" };

	auto mesh = std::shared_ptr<Mesh>{ new Mesh, [](Mesh* _p) {
		delete _p;
	} };

	mesh->_CreateMesh(_topology, _vtx_data, _vtx_stride, _vtx_count, _vtx_usage, _idx_data, _idx_stride, _idx_count, _idx_usage, _idx_format);

	mesh_map_.insert(std::make_pair(_tag, std::move(mesh)));
}

void K::ResourceManager::_CreateMesh(
	std::string const& _tag, D3D11_PRIMITIVE_TOPOLOGY _topology,
	void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage,
	void* _idx_data, int _idx_stride, int _idx_count, D3D11_USAGE _idx_usage, DXGI_FORMAT _idx_format,
	void* _inst_data, int _inst_stride, int _inst_count, D3D11_USAGE _inst_usage)
{
	if(FindMesh(_tag))
		throw std::exception{ "ResourceManager::_CreateMesh" };

	auto mesh = std::shared_ptr<Mesh>{ new Mesh, [](Mesh* _p) {
		delete _p;
	} };

	mesh->_CreateMesh(_topology, _vtx_data, _vtx_stride, _vtx_count, _vtx_usage, _idx_data, _idx_stride, _idx_count, _idx_usage, _idx_format, _inst_data, _inst_stride, _inst_count, _inst_usage);

	mesh_map_.insert(std::make_pair(_tag, std::move(mesh)));
}

void K::ResourceManager::_CreateTexture2D(std::string const& _tag, std::wstring const& _file_name, std::string const& _path_tag)
{
	if (FindTexture(_tag))
		throw std::exception{ "ResourceManager::_CreateTexture2D" };

	auto texture = std::shared_ptr<Texture>{ new Texture, [](Texture* _p) {
		delete _p;
	} };

	texture->_CreateTexture2D(_file_name, _path_tag);

	texture_map_.insert(std::make_pair(_tag, std::move(texture)));
}

void K::ResourceManager::_CreateSampler(
	std::string const& _tag,
	D3D11_FILTER _filter,
	D3D11_TEXTURE_ADDRESS_MODE _address_u,
	D3D11_TEXTURE_ADDRESS_MODE _address_v,
	D3D11_TEXTURE_ADDRESS_MODE _address_w)
{
	if (FindSampler(_tag))
		throw std::exception{ "ResourceManager::_CreateSampler" };

	auto sampler = std::shared_ptr<Sampler>{ new Sampler, [](Sampler* _p) {
		delete _p;
	} };

	sampler->_CreateSampler(_filter, _address_u, _address_v, _address_w);

	sampler_map_.insert(std::make_pair(_tag, std::move(sampler)));
}

void K::ResourceManager::_CreateAnimation2DClip(std::string const& _tag, std::wstring const& _file_name, std::string const& _path_tag)
{
	std::filesystem::path path_buffer = PathManager::singleton()->FindPath(_path_tag);

	if (path_buffer.empty())
		throw std::exception{ "ResourceManager::_CreateAnimation2DClip" };

	path_buffer /= _file_name;

	std::ifstream file{ path_buffer };

	if(file.fail())
		throw std::exception{ "ResourceManager::_CreateAnimation2DClip" };

	std::string line{};
	std::stringstream line_stream{};

	std::getline(file, line);
	std::getline(file, line);
	line_stream.str(line);
	std::getline(file, line);

	std::string clip_tag{};
	std::string texture_tag{};
	std::string type{};
	std::string option{};
	std::string width{};
	std::string height{};
	std::string completion_time{};

	std::getline(line_stream, clip_tag, ',');
	std::getline(line_stream, texture_tag, ',');
	std::getline(line_stream, type, ',');
	std::getline(line_stream, option, ',');
	std::getline(line_stream, width, ',');
	std::getline(line_stream, height, ',');
	std::getline(line_stream, completion_time, ',');

	if(nullptr == FindTexture(clip_tag))
		throw std::exception{ "ResourceManager::_CreateAnimation2DClip" };

	auto animation_2d_clip = std::make_shared<ANIMATION_2D_CLIP_DESC>();
	animation_2d_clip->clip_tag = clip_tag;
	animation_2d_clip->texture_tag = texture_tag;
	animation_2d_clip->type = static_cast<ANIMATION_2D_TYPE>(std::stoi(type));
	animation_2d_clip->option = static_cast<ANIMATION_OPTION>(std::stoi(option));
	animation_2d_clip->width = std::stof(width);
	animation_2d_clip->height = std::stof(height);
	animation_2d_clip->completion_time = std::stof(completion_time);

	while (true)
	{
		line.clear();
		line_stream.clear();

		std::string left{};
		std::string top{};
		std::string right{};
		std::string bottom{};

		getline(file, line);
		getline(file, line);
		line_stream.str(line);

		if (line.empty())
			break;

		getline(line_stream, left, ',');
		getline(line_stream, top, ',');
		getline(line_stream, right, ',');
		getline(line_stream, bottom, ',');

		ANIMATION_2D_FRAME_DESC frame{};
		frame.LT = Vector2{ std::stof(left), std::stof(top) };
		frame.RB = Vector2{ std::stof(right), std::stof(bottom) };

		animation_2d_clip->frame_vector.push_back(std::move(frame));
	}
}
