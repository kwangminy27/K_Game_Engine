#pragma once

namespace K
{
	class K_COMMON_DLL Tag
	{
	public:
		std::string const& tag() const;

		void set_tag(std::string const& _tag);

	protected:
		Tag() = default;
		Tag(Tag const& _other);
		Tag(Tag&& _other) noexcept;
		Tag& operator=(Tag const&) = default;
		Tag& operator=(Tag&&) noexcept = default;

		std::string tag_{};
	};
}
