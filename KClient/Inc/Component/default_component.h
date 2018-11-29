#pragma once

namespace K
{
	class DefaultComponent final : public Component
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual CPTR Clone() const override;

	private:
		DefaultComponent() = default;
		DefaultComponent(DefaultComponent const& _other);
		DefaultComponent(DefaultComponent&& _other) noexcept;
		DefaultComponent& operator=(DefaultComponent const&) = delete;
		DefaultComponent& operator=(DefaultComponent&&) noexcept = default;

		virtual void _Finalize() override;
	};
}
