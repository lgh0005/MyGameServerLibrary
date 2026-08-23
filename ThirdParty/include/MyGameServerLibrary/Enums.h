#pragma once
#include "MyGameShared/Types.h"

namespace MGSL::Net
{
	enum class NetIOCPEventType : Shared::uint8
	{
		Connect,
		Disconnect,
		Accept,
		Recv,
		Send
	};

	enum class NetServiceType : Shared::uint8 
	{
		Server,
		Client
	};

	enum class EPacketID : Shared::uint16
	{
		S_TEST = 1,
		S_EnterGame = 2,
		S_Spawn = 3,
		S_SyncObjects = 4,

		C_EnterGame = 10,
		C_Move = 11,
		C_Jump = 12,
		C_ChangeWeapon = 13,
		C_Attack = 14
	};
}