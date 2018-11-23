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

		static std::shared_ptr<Mesh> mesh_nullptr_;
		static std::shared_ptr<Texture> texture_nullptr_;
		static std::shared_ptr<Sampler> sampler_nullptr_;
		std::unordered_map<std::string, std::shared_ptr<Mesh>> mesh_map_{};
		std::unordered_map<std::string, std::shared_ptr<Texture>> texture_map_{};
		std::unordered_map<std::string, std::shared_ptr<Sampler>> sampler_map_{};
	};
}
