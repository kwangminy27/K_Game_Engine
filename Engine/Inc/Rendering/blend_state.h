#pragma once

#include "state.h"

namespace K
{
	class K_ENGINE_DLL BlendState final : public State
	{
		friend class RenderingManager;
	public:
		virtual void SetToShader() override;

	protected:
		BlendState() = default;
		BlendState(BlendState const& _other);
		BlendState(BlendState&& _other) noexcept;
		BlendState& operator=(BlendState const&) = default;
		BlendState& operator=(BlendState&&) noexcept = default;

		void _CreateState(
			bool _alpha_to_coverage_enable,
			bool _independent_blend_enable,
			std::vector<D3D11_RENDER_TARGET_BLEND_DESC> const& _render_target_blend_desc_vector);
	};
}
