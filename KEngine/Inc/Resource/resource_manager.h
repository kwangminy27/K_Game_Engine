#pragma once

namespace K
{
	class Mesh;
	class Texture;
	class Sampler;

	class K_ENGINE_DLL ResourceManager final : public Singleton<ResourceManager>
	{
		friend class Singleton<ResourceManager>;
	public:
		virtual void Initialize() override;

		std::shared_ptr<Mesh> const& FindMesh(std::string const& _tag) const;
		std::shared_ptr<Texture> const& FindTexture(std::string const& _tag) const;
		std::shared_ptr<Sampler> const& FindSampler(std::string const& _tag) const;

	private:
		ResourceManager() = default;
		ResourceManager(ResourceManager const&) = delete;
		ResourceManager(ResourceManager&&) noexcept = delete;
		ResourceManager& operator=(ResourceManager const&) = delete;
		ResourceManager& operator=(ResourceManager&&) noexcept = delete;

		virtual void _Finalize() override;

		void _CreateMesh(
			std::string const& _tag, D3D11_PRIMITIVE_TOPOLOGY _topology,
			void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage);
		void _CreateMesh(
			std::string const& _tag, D3D11_PRIMITIVE_TOPOLOGY _topology,
			void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage,
			void* _idx_data, int _idx_stride, int _idx_count, D3D11_USAGE _idx_usage, DXGI_FORMAT _idx_format);
		void _CreateMesh(
			std::string const& _tag, D3D11_PRIMITIVE_TOPOLOGY _topology,
			void* _vtx_data, int _vtx_stride, int _vtx_count, D3D11_USAGE _vtx_usage,
			void* _idx_data, int _idx_stride, int _idx_count, D3D11_USAGE _idx_usage, DXGI_FORMAT _idx_format,
			void* _inst_data, int _inst_stride, int _inst_count, D3D11_USAGE _inst_usage);
		void _CreateTexture2D(std::string const& _tag, std::wstring const& _file_name, std::string const& _path_tag);
		void _CreateSampler(
			std::string const& _tag,
			D3D11_FILTER _filter,
			D3D11_TEXTURE_ADDRESS_MODE _address_u,
			D3D11_TEXTURE_ADDRESS_MODE _address_v,
			D3D11_TEXTURE_ADDRESS_MODE _address_w);

		static std::shared_ptr<Mesh> mesh_nullptr_;
		static std::shared_ptr<Texture> texture_nullptr_;
		static std::shared_ptr<Sampler> sampler_nullptr_;
		std::unordered_map<std::string, std::shared_ptr<Mesh>> mesh_map_{};
		std::unordered_map<std::string, std::shared_ptr<Texture>> texture_map_{};
		std::unordered_map<std::string, std::shared_ptr<Sampler>> sampler_map_{};
	};
}
