#pragma once

namespace K
{
	class Layer;

	class K_ENGINE_DLL Level : public Tag, public std::enable_shared_from_this<Level>
	{
		friend class WorldManager;
	public:
		virtual void Initialize() override;

		std::shared_ptr<Layer> const& FindLayer(TAG const& _tag) const;

		static std::shared_ptr<Layer> layer_dummy_;

	protected:
		Level() = default;
		Level(Level const& _other);
		Level(Level&& _other) noexcept;
		Level& operator=(Level const&) = default;
		Level& operator=(Level&&) noexcept = default;

		virtual void _Finalize() override;

		void _Input(float _time);
		void _Update(float _time);
		void _Collision(float _time);
		void _Render(float _time);

		void _CreateLayer(std::string const& _tag, uint32_t _order);

	private:
		std::list<std::shared_ptr<Layer>> layer_list_{};
	};
}