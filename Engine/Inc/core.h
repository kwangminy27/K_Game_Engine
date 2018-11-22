#pragma once

#include <KCommon.h>
#include <singleton.h>

namespace K
{
	class K_ENGINE_DLL Core : Singleton<Core>
	{
		friend class Singleton<Core>;
	public:
		virtual void Initialize() override;
		virtual void Finalize() override;

	protected:
		Core() = default;
		Core(Core const&) = delete;
		Core(Core&&) noexcept = delete;
		Core& operator=(Core const&) = delete;
		Core& operator=(Core&&) noexcept = delete;
	};
}
