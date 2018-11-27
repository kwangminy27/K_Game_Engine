#include "KEngine.h"
#include "video_manager.h"

#include "path_manager.h"

void K::VideoManager::Initialize()
{
	Play(L"BlizNorth640x480.avi");
}

void K::VideoManager::Play(std::wstring const& _file_name)
{
	ThrowIfFailed(CoCreateInstance(CLSID_FilterGraph, nullptr, CLSCTX_INPROC_SERVER, __uuidof(IGraphBuilder), &graph_builder_));

	std::filesystem::path path_buffer = PathManager::singleton()->FindPath(VIDEO_PATH);

	path_buffer /= _file_name;

	ThrowIfFailed(graph_builder_->RenderFile(path_buffer.c_str(), nullptr));

	ThrowIfFailed(graph_builder_.As(&media_control_));
	ThrowIfFailed(graph_builder_.As(&video_window_));

	ThrowIfFailed(video_window_->put_FullScreenMode(OATRUE));
	ThrowIfFailed(media_control_->Run());
}

void K::VideoManager::Stop()
{
	video_window_.Reset();
	media_control_.Reset();
	graph_builder_.Reset();
}

void K::VideoManager::_Finalize()
{
}
