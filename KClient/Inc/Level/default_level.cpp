#include "KClient.h"
#include "default_level.h"

#include "Actor/default_actor.h"
#include "Component/default_component.h"

void K::DefaultLevel::Initialize()
{
	try
	{
		auto const& object_manager = ObjectManager::singleton();

		auto layer = CreateLayer({ "DefaultLayer", 0 });

		auto actor = object_manager->CreateActor<DefaultActor>({ "DefaultActor", 0 });
		layer->AddActor(actor);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "DefaultLevel::Initialize" << std::endl;
	}
}

K::DefaultLevel::DefaultLevel(DefaultLevel&& _other) noexcept : Level(std::move(_other))
{
}

void K::DefaultLevel::_Finalize()
{
}
