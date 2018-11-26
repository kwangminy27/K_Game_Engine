#pragma once

namespace K
{
	struct CSO_DESC;

	class Shader;
	class State;

	struct ConstantBuffer
	{
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		int size;
		int shader_flag;
		int slot;
	};

	class K_ENGINE_DLL RenderingManager : public Singleton<RenderingManager>
	{
		friend class Singleton<RenderingManager>;
	public:
		virtual void Initialize() override;

		std::shared_ptr<Shader> const& FindShader(std::string const& _tag) const;
		std::shared_ptr<State> const& FindState(std::string const& _tag) const;
		std::shared_ptr<ConstantBuffer> const& FindConstantBuffer(std::string const& _tag) const;

		void UpdateConstantBuffer(std::string const& _tag, void* _data);

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
		void _CreateConstantBuffer(std::string const& _tag, int _size, int _shader_flag, int _slot);

		static std::shared_ptr<Shader> shader_nullptr_;
		static std::shared_ptr<State> state_nullptr_;
		static std::shared_ptr<ConstantBuffer> CB_nullptr_;
		std::unordered_map<std::string, std::shared_ptr<Shader>> shader_map_{};
		std::unordered_map<std::string, std::shared_ptr<State>> state_map_{};
		std::unordered_map<std::string, std::shared_ptr<ConstantBuffer>> CB_map_{};
	};
}
