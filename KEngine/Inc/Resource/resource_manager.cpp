#include "KEngine.h"
#include "resource_manager.h"

#include "mesh.h"
#include "texture.h"
#include "sampler.h"

std::shared_ptr<K::Mesh> K::ResourceManager::mesh_nullptr_{};
std::shared_ptr<K::Texture> K::ResourceManager::texture_nullptr_{};
std::shared_ptr<K::Sampler> K::ResourceManager::sampler_nullptr_{};

void K::ResourceManager::Initialize()
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
		std::cout << "ResourceManager::Initialize" << std::endl;
	}
}

std::shared_ptr<K::Mesh> const& K::ResourceManager::FindMesh(std::string const& _tag) const
{
	auto iter = mesh_map_.find(_tag);

	if (iter == mesh_map_.end())
		return mesh_nullptr_;

	return iter->second;
}

std::shared_ptr<K::Texture> const& K::ResourceManager::FindTexture(std::string const& _tag) const
{
	auto iter = texture_map_.find(_tag);

	if (iter == texture_map_.end())
		return texture_nullptr_;

	return iter->second;
}

std::shared_ptr<K::Sampler> const& K::ResourceManager::FindSampler(std::string const& _tag) const
{
	auto iter = sampler_map_.find(_tag);

	if (iter == sampler_map_.end())
		return sampler_nullptr_;

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

	texture->_CreateTexture2D(_tag, _file_name, _path_tag);

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

	sampler->_CreateSampler(_tag, _filter, _address_u, _address_v, _address_w);

	sampler_map_.insert(std::make_pair(_tag, std::move(sampler)));
}
