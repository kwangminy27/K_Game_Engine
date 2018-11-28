#pragma once

namespace K
{
	class Level;

	class K_ENGINE_DLL Layer final : public Tag, public std::enable_shared_from_this<Layer>
	{
		friend class Level;
	public:
		virtual void Initialize() override;

		uint32_t order() const;
		std::shared_ptr<Level> level() const;

		void set_order(uint32_t _order);
		void set_level(std::shared_ptr<Level> const& _level);

	private:
		Layer() = default;
		Layer(Layer const& _other);
		Layer(Layer&& _other) noexcept;
		Layer& operator=(Layer const&) = default;
		Layer& operator=(Layer&&) noexcept = default;

		virtual void _Finalize() override;

		void _Input(float _time);
		void _Update(float _time);
		void _Collision(float _time);
		void _Render(float _time);

		uint32_t order_{};
		std::weak_ptr<Level> level_{};
	};
}
