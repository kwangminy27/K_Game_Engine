#include "KEngine.h"
#include "core.h"

void K::Core::Initialize()
{
}

void K::Core::Initialize(std::wstring const& _class_name, std::wstring const& _window_name, HINSTANCE _instance)
{
	try
	{
		instance_ = _instance;

		_RegisterClass(_class_name);
		_CreateWindow(_class_name, _window_name);
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
	while (true)
	{
		if (PeekMessage(&message, nullptr, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
			_Logic();
	}
}

void K::Core::_Finalize()
{
}

LRESULT K::Core::_WindowProc(HWND _window, UINT _message, WPARAM _w_param, LPARAM _l_param)
{
	switch (_message)
	{
	case WM_KEYDOWN:
		switch (_w_param)
		{
		case VK_ESCAPE:
			DestroyWindow(_window);
			return 0;
		}
		return 0;

	case WM_DESTROY:
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

void K::Core::_Logic()
{
	float delta_time{};

	_Input(delta_time);
	_Update(delta_time);
	_Collision(delta_time);
	_Render(delta_time);
}

void K::Core::_Input(float _time)
{
}

void K::Core::_Update(float _time)
{
}

void K::Core::_Collision(float _time)
{
}

void K::Core::_Render(float _time)
{
}
