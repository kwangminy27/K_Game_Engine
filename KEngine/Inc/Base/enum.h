#pragma once

namespace K
{
	enum class RESOLUTION
	{
		WIDTH = 1600,
		HEIGHT = 900
	};

	enum class SHADER_TYPE
	{
		VERTEX = 1 << 0,
		PIXEL = 1 << 1
	};

	enum class PACKET_TYPE
	{
		CONNECT,
		REPLICATION,
		RPC,
		DISCONNECT
	};
}
