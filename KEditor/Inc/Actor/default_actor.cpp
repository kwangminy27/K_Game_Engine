#include "stdafx.h"
#include "default_actor.h"

#include "Component/default_component.h"

void K::DefaultActor::Initialize()
{
	try
	{
		auto const& object_manager = ObjectManager::singleton();

		auto component = object_manager->CreateComponent<DefaultComponent>({ "DefaultComponent", 0 });
		AddComponent(component);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "DefaultActor::Initialize" << std::endl;
	}
}

K::APTR K::DefaultActor::Clone() const
{
	return APTR{ new DefaultActor{ *this }, [](DefaultActor* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

K::DefaultActor::DefaultActor(DefaultActor const& _other) : Actor(_other)
{
}

K::DefaultActor::DefaultActor(DefaultActor&& _other) noexcept : Actor(std::move(_other))
{
}

void K::DefaultActor::_Finalize()
{
}
