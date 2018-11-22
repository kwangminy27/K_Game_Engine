#pragma once

namespace K
{
	template <typename T>
	class K_COMMON_DLL Singleton
	{
	public:
		static std::unique_ptr<T, std::function<void(T*)>> const& singleton();

		virtual void Initialize() = 0;
		virtual void Finalize() = 0;

	protected:
		Singleton() = default;
		Singleton(Singleton const&) = delete;
		Singleton(Singleton&&) noexcept = delete;
		Singleton& operator=(Singleton const&) = delete;
		Singleton& operator=(Singleton&&) noexcept = delete;

		static std::unique_ptr<T, std::function<void(T*)>> singleton_;
		static std::once_flag once_flag_;
	};
}

#include "singleton.inl"