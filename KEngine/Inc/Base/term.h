#pragma once

namespace K
{
	class Actor;
	class Component;

	using APTR = std::shared_ptr<Actor>;
	using CPTR = std::unique_ptr<Component, std::function<void(Component*)>>;

	template <typename T> constexpr auto CPTR_CAST(CPTR const& p) { return static_cast<T*>(p.get()); }

	//////////////////// Path Key ////////////////////
	constexpr auto ROOT_PATH = "RootPath";
	constexpr auto TEXTURE_PATH = "TexturePath";
	constexpr auto AUDIO_PATH = "AudioPath";
	constexpr auto VIDEO_PATH = "VideoPath";

	struct TransformConstantBuffer
	{
		Matrix world;
		Matrix view;
		Matrix projection;
		Matrix WVP;
	};

	struct MaterialConstantBuffer
	{
		Vector4 diffuse;
	};
}
