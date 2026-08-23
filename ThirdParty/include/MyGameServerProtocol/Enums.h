#pragma once

namespace MGSL::Protocol
{
	enum class PacketID : Shared::uint8
	{
		S_TEST = 1,
		S_EnterGame = 2,
		S_Spawn = 3,
		S_SyncObjects = 4,

		C_EnterGame = 10,
		C_Move = 11,
		C_Jump = 12,
		C_ChangeWeapon = 13
	};
}