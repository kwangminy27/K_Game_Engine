#include "KCommon.h"
#include "tag.h"

std::string const& K::Tag::tag() const
{
	return tag_;
}

void K::Tag::set_tag(std::string const& _tag)
{
	tag_ = _tag;
}

K::Tag::Tag(Tag const& _other)
{
	*this = _other;
}

K::Tag::Tag(Tag&& _other) noexcept
{
	*this = std::move(_other);
}
