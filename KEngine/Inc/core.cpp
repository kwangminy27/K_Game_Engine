#include "KEngine.h"
#include "core.h"

#include "device_manager.h"
#include "text_manager.h"
#include "path_manager.h"
#include "Resource/resource_manager.h"
#include "Rendering/rendering_manager.h"
#include "Audio/audio_manager.h"
#include "Video/video_manager.h"
#include "time_manager.h"
#include "input_manager.h"
#include "World/world_manager.h"
#include "Object/object_manager.h"
#include "collision_manager.h"
#include "registry_manager.h"
#include "replication_manager.h"
#include "connection_manager.h"

bool K::Core::shutdown_{};

void K::Core::Initialize()
{
}

void K::Core::Initialize(std::wstring const& _class_name, std::wstring const& _window_name, HINSTANCE _instance)
{
	try
	{
		_RegisterClass(_class_name);
		_CreateWindow(_class_name, _window_name);

		Initialize(_instance, window_);
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Core::Initialize" << std::endl;
	}
}

void K::Core::Initialize(HINSTANCE _instance, HWND _window)
{
	try
	{
		ThrowIfFailed(CoInitializeEx(nullptr, COINIT_MULTITHREADED));

		instance_ = _instance;
		window_ = _window;

		SocketManager::singleton()->Initialize();

		DeviceManager::singleton()->Initialize(window_);
		TextManager::singleton()->Initialize();
		PathManager::singleton()->Initialize();
		VideoManager::singleton()->Initialize();
		AudioManager::singleton()->Initialize();
		ResourceManager::singleton()->Initialize();
		RenderingManager::singleton()->Initialize();
		TimeManager::singleton()->Initialize();
		InputManager::singleton()->Initialize();
		WorldManager::singleton()->Initialize();
		ObjectManager::singleton()->Initialize();
		CollisionManager::singleton()->Initialize();
		RegistryManager::singleton()->Initialize();
		ReplicationManager::singleton()->Initialize();
		ConnectionManager::singleton()->Initialize();
	}
	catch (std::exception const& _e)
	{
		std::cout << _e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Core::Initialize" << std::endl;
	}
}

void K::Core::Run()
{
	MSG message{};
	while (!shutdown_)
	{
		if (PeekMessage(&message, nullptr, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
			Logic();
	}
}

void K::Core::Logic()
{
	auto const& time_manager = TimeManager::singleton();

	time_manager->Update();

	float time_delta = time_manager->time_delta();

	_Input(time_delta);
	_Update(time_delta);
	_Collision(time_delta);
	_Render(time_delta);
}

void K::Core::_Finalize()
{
	ConnectionManager::singleton().reset();
	ReplicationManager::singleton().reset();
	RegistryManager::singleton().reset();
	CollisionManager::singleton().reset();
	ObjectManager::singleton().reset();
	WorldManager::singleton().reset();
	InputManager::singleton().reset();
	TimeManager::singleton().reset();
	RenderingManager::singleton().reset();
	ResourceManager::singleton().reset();
	AudioManager::singleton().reset();
	VideoManager::singleton().reset();
	PathManager::singleton().reset();
	TextManager::singleton().reset();
	DeviceManager::singleton().reset();

	SocketManager::singleton().reset();

	CoUninitialize();
}

LRESULT K::Core::_WindowProc(HWND _window, UINT _message, WPARAM _w_param, LPARAM _l_param)
{
	switch (_message)
	{
	case WM_DESTROY:
		Core::shutdown_ = true;
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(_window, _message, _w_param, _l_param);
}

void K::Core::_RegisterClass(std::wstring const& _class_name)
{
	WNDCLASSEX wcex{};
	wcex.cbSize = sizeof(wcex);
	wcex.lpfnWndProc = K::Core::_WindowProc;
	wcex.hInstance = instance_;
	wcex.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	wcex.lpszClassName = _class_name.c_str();
	if (!RegisterClassEx(&wcex))
		throw std::exception{ "Core::_RegisterClass" };
}

void K::Core::_CreateWindow(std::wstring const& _class_name, std::wstring const& _window_name)
{
	window_ = CreateWindowEx(
		NULL,
		_class_name.c_str(),
		_window_name.c_str(),
		WS_CAPTION | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		nullptr,
		nullptr,
		instance_,
		nullptr
	);

	if (!window_)
		throw std::exception{ "Core::_CreateWindow" };

	RECT rc{ 0, 0, static_cast<int>(RESOLUTION::WIDTH) , static_cast<int>(RESOLUTION::HEIGHT) };
	AdjustWindowRectEx(&rc, WS_CAPTION | WS_SYSMENU, false, NULL);

	MoveWindow(window_, 152, 70, rc.right - rc.left, rc.bottom - rc.top, true);
	ShowWindow(window_, SW_SHOW);
}

void K::Core::_Input(float _time)
{
	auto const& input_manager = InputManager::singleton();

	input_manager->Update();

	WorldManager::singleton()->Input(_time);
}

void K::Core::_Update(float _time)
{
	WorldManager::singleton()->Update(_time);
}

void K::Core::_Collision(float _time)
{
	WorldManager::singleton()->Collision(_time);
}

void K::Core::_Render(float _time)
{
	auto const& device_manager = DeviceManager::singleton();

	device_manager->Clear();

	WorldManager::singleton()->Render(_time);

	device_manager->Present();
}
