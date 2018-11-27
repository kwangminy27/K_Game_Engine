#pragma once

#ifdef K_ENGINE_EXPORT
#define K_ENGINE_DLL __declspec(dllexport)
#else
#define K_ENGINE_DLL __declspec(dllimport)
#endif

//////////////////// Path Key ////////////////////
constexpr auto ROOT_PATH = "RootPath";
constexpr auto TEXTURE_PATH = "TexturePath";
constexpr auto AUDIO_PATH = "AudioPath";
constexpr auto VIDEO_PATH = "VideoPath";