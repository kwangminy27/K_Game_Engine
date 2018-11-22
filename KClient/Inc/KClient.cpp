#include <KEngine.h>
#include <core.h>

int WINAPI wWinMain(HINSTANCE _instance, HINSTANCE _prev_instance, PWSTR _cmd_line, int _cmd_show)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	auto& core = K::Core::singleton();

	core->Initialize(L"K Game Engine", L"K Game Engine", _instance);
	core->Run();

	core.reset();

	return 0;
}