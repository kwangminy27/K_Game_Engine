#pragma once

namespace K
{
	class Shader;
	class RenderState;

	struct ConstantBuffer
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		uint32_t size;
		uint32_t shader_flag;
		uint32_t slot;
	};

	class K_ENGINE_DLL RenderingManager : public Singleton<RenderingManager>
	{
		friend class Singleton<RenderingManager>;
	public:
		virtual void Initialize() override;

		std::shared_ptr<Shader> const& FindShader(std::string const& _tag) const;
		std::shared_ptr<RenderState> const& FindRenderState(std::string const& _tag) const;
		std::shared_ptr<ConstantBuffer> const& FindConstantBuffer(std::string const& _tag) const;

		void UpdateConstantBuffer(std::string const& _tag, void* _data);

		static std::shared_ptr<Shader> shader_dummy_;
		static std::shared_ptr<RenderState> render_state_dummy_;
		static std::shared_ptr<ConstantBuffer> CB_dummy_;

	private:
		RenderingManager() = default;
		RenderingManager(RenderingManager const&) = delete;
		RenderingManager(RenderingManager&&) noexcept = delete;
		RenderingManager& operator=(RenderingManager const&) = delete;
		RenderingManager& operator=(RenderingManager&&) noexcept = delete;

		virtual void _Finalize() override;

		void _CreateShader(
			std::string const& _tag,
			std::vector<CSO_DESC> const& _cso_desc_vector,
			std::vector<D3D11_INPUT_ELEMENT_DESC> const& _input_element_desc_vector,
			std::string const& _path_tag);
		void _CreateDepthStencilState(
			std::string const& _tag,
			BOOL _depth_enable,
			D3D11_DEPTH_WRITE_MASK _depth_write_mask,
			D3D11_COMPARISON_FUNC _depth_func,
			BOOL _stencil_enable,
			UINT8 _stencil_read_mask,
			UINT8 _stencil_write_mask,
			D3D11_DEPTH_STENCILOP_DESC const& _front_face,
			D3D11_DEPTH_STENCILOP_DESC const& _back_face);
		void _CreateBlendState(
			std::string const& _tag,
			bool _alpha_to_coverage_enable,
			bool _independent_blend_enable,
			std::vector<D3D11_RENDER_TARGET_BLEND_DESC> const& _render_target_blend_desc_vector);
		void _CreateConstantBuffer(std::string const& _tag, uint32_t _size, uint8_t _shader_flag, uint32_t _slot);

		std::unordered_map<std::string, std::shared_ptr<Shader>> shader_map_{};
		std::unordered_map<std::string, std::shared_ptr<RenderState>> render_state_map_{};
		std::unordered_map<std::string, std::shared_ptr<ConstantBuffer>> CB_map_{};
	};
}
