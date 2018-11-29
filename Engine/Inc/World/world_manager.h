#pragma once

namespace K
{
	class Level;
	class Layer;

	class K_ENGINE_DLL WorldManager final : public Singleton<WorldManager>
	{
		friend class Singleton<WorldManager>;
	public:
		virtual void Initialize() override;

		void Input(float _time);
		void Update(float _time);
		void Collision(float _time);
		void Render(float _time);

		std::shared_ptr<Level> const& FindLevel(TAG const& _tag) const;
		std::shared_ptr<Layer> const& FindLayer(TAG const& _tag) const;
		APTR const& FindActor(TAG const& _tag) const;

		static std::shared_ptr<Level> level_dummy_;

	private:
		WorldManager() = default;
		WorldManager(WorldManager const&) = delete;
		WorldManager(WorldManager&&) noexcept = delete;
		WorldManager& operator=(WorldManager const&) = delete;
		WorldManager& operator=(WorldManager&&) noexcept = delete;

		virtual void _Finalize() override;

		template <typename T> void _CreateLevel(TAG const& _tag);

		std::list<std::shared_ptr<Level>> level_list_{};
	};
}

#include "world_manager.inl"
