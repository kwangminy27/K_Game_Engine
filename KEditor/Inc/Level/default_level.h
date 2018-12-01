#pragma once

namespace K
{
	class DefaultLevel final : public Level
	{
		friend class WorldManager;
	public:
		virtual void Initialize() override;

	private:
		DefaultLevel() = default;
		DefaultLevel(DefaultLevel const&) = delete;
		DefaultLevel(DefaultLevel&& _other) noexcept;
		DefaultLevel& operator=(DefaultLevel const&) = delete;
		DefaultLevel& operator=(DefaultLevel&&) noexcept = default;

		virtual void _Finalize() override;
	};
}
