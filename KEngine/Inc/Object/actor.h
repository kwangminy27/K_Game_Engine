#pragma once

namespace K
{
	class Level;
	class Layer;
	class Component;

	class K_ENGINE_DLL Actor : public Tag, public std::enable_shared_from_this<Actor>
	{
	public:
		virtual void Initialize() override;

		virtual void Input(float _time);
		virtual void Update(float _time);
		virtual void Collision(float _time);
		virtual void Render(float _time);

		virtual APTR Clone() const = 0;

		CPTR const& FindComponent(TAG const& _tag) const;
		APTR const& FindChild(TAG const& _tag) const;

		void AddComponent(CPTR _component);
		void AddChild(APTR const& _child);

		void RemoveComponent(TAG const& _tag);
		void RemoveChild(TAG const& _tag);

		std::shared_ptr<Level> level() const;
		std::shared_ptr<Layer> layer() const;

		void set_level(std::shared_ptr<Level> const& _level);
		void set_layer(std::shared_ptr<Layer> const& _layer);

		static CPTR component_dummy_;
		static APTR child_dummy_;

	protected:
		Actor() = default;
		Actor(Actor const& _other);
		Actor(Actor&& _other) noexcept;
		Actor& operator=(Actor const&) = delete;
		Actor& operator=(Actor&&) noexcept = default;

		virtual void _Finalize() override;

		std::weak_ptr<Level> level_{};
		std::weak_ptr<Layer> layer_{};
		std::list<CPTR> component_list_{};
		std::list<APTR> child_list_{};
	};
}
