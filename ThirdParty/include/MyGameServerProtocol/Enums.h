#pragma once

namespace MGSL::Protocol
{
	enum class PacketID : Shared::uint8
	{
		S_TEST = 1,
		S_EnterGame = 2,
		S_SpawnPlayer = 3,
		S_SyncPlayers = 4,
		S_RemovePlayer = 5,
		S_SpawnBullet = 6,
		S_SyncBullets = 7,
		S_RemoveBullet = 8,
		S_SpawnEffect = 9,

		C_EnterGame = 10,
		C_Move = 11,
		C_Jump = 12,
		C_ChangeWeapon = 13,
		C_Attack = 14
	};
}
