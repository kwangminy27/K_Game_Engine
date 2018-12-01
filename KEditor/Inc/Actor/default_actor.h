#pragma once

namespace K
{
	class DefaultActor final : public Actor
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

	private:
		DefaultActor() = default;
		DefaultActor(DefaultActor const& _other);
		DefaultActor(DefaultActor&& _other) noexcept;
		DefaultActor& operator=(DefaultActor const&) = delete;
		DefaultActor& operator=(DefaultActor&&) noexcept = default;

		virtual void _Finalize() override;
	};
}
