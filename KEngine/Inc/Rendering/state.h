#pragma once

namespace K
{
	class K_ENGINE_DLL State
	{
		friend class RenderingManager;
	public:
		virtual void SetToShader() = 0;

	protected:
		State() = default;
		State(State const& _other);
		State(State&& _other) noexcept;
		State& operator=(State const&) = default;
		State& operator=(State&&) noexcept = default;
		virtual ~State() = default;

		Microsoft::WRL::ComPtr<ID3D11DeviceChild> state_{};
	};
}
