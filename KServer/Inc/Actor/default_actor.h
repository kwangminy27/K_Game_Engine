#pragma once

namespace K
{
	class DefaultActor final : public ActorServer
	{
		friend class ObjectManager;
	public:
		virtual void Initialize() override;

		virtual APTR Clone() const override;

		virtual void Serialize(InputMemoryStream& _imstream) override;
		virtual void Serialize(OutputMemoryStream& _omstream) override;

	private:
		DefaultActor() = default;
		DefaultActor(DefaultActor const& _other);
		DefaultActor(DefaultActor&& _other) noexcept;
		DefaultActor& operator=(DefaultActor const&) = delete;
		DefaultActor& operator=(DefaultActor&&) noexcept = default;

		virtual void _Finalize() override;

		virtual void _Input(float _time) override;
		virtual void _Update(float _time) override;
		virtual void _Collision(float _time) override;
		virtual void _Render(float _time) override;
	};
}
