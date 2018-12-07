#include "KEngine.h"
#include "camera_actor.h"

#include "Object/object_manager.h"
#include "Object/Component/camera.h"

void K::CameraActor::Initialize()
{
	try
	{
		auto const& object_manager = ObjectManager::singleton();

		auto camera = object_manager->CreateComponent<Camera>({ "Camera", 0 });
		AddComponent(camera);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "CameraActor::Initialize" << std::endl;
	}
}

K::APTR K::CameraActor::Clone() const
{
	return APTR{ new CameraActor{ *this }, [](CameraActor* _p) {
		_p->_Finalize();
		delete _p;
	} };
}

void K::CameraActor::Serialize(InputMemoryStream& _imstream)
{
}

void K::CameraActor::Serialize(OutputMemoryStream& _omstream)
{
}

K::Matrix const& K::CameraActor::view() const
{
	return CPTR_CAST<Camera>(FindComponent({ "Camera", 0 }))->view();
}

K::Matrix const& K::CameraActor::projection() const
{
	return CPTR_CAST<Camera>(FindComponent({ "Camera", 0 }))->projection();
}

K::CameraActor::CameraActor(CameraActor const& _other) : Actor(_other)
{
}

K::CameraActor::CameraActor(CameraActor&& _other) noexcept : Actor(std::move(_other))
{
}

void K::CameraActor::_Finalize()
{
}

void K::CameraActor::_Input(float _time)
{
}

void K::CameraActor::_Update(float _time)
{
}

void K::CameraActor::_Collision(float _time)
{
}

void K::CameraActor::_Render(float _time)
{
}
