#pragma once

namespace MGSL::Protocol
{
	struct PacketHeader
	{
		Shared::uint16 size;
		Shared::uint16 id;		// 프로토콜ID (ex. 1=로그인, 2=이동요청)
	};
}