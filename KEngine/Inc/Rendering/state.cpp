#include "KEngine.h"
#include "state.h"

K::State::State(State const& _other)
{
	*this = _other;
}

K::State::State(State&& _other) noexcept
{
	*this = std::move(_other);
}
