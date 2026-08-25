#pragma once
#include "MyGameServerLibrary/PacketHandler.h"

namespace MGSL::Net
{
	MGSL_CLASS_PTR(SendBuffer)
	MGSL_CLASS_PTR(PacketSession)
	MGSL_CLASS_PTR(GameSession)

	class ServerPacketHandler final : public Net::PacketHandler<ServerPacketHandler>
	{
		MGSL_DISABLE_COPY(ServerPacketHandler)
		MGSL_DISABLE_MOVE(ServerPacketHandler)

	public:
		static void HandlePacketImpl
		(
			PacketSessionPtr session,
			BYTE* buffer, 
			Shared::int32 len
		);

	/*=================================//
	//    default packet generators    //
	//=================================*/
	public:
		static SendBufferPtr Create_S_EnterGame(bool success, const ::Protobuf::PlayerInfo& player);

	/*========================//
	//   Packet Test Methods  //
	//========================*/
	public:
		/* Handle */
		static void Handle_C_MOVE(GameSessionPtr session, BYTE* buffer, Shared::int32 len);
		static void Handle_C_ENTER_GAME(GameSessionPtr session, BYTE* buffer, Shared::int32 len);
		static void Handle_C_JUMP(GameSessionPtr session, BYTE* buffer, Shared::int32 len);
		static void Handle_C_CHANGE_WEAPON(GameSessionPtr session, BYTE* buffer, Shared::int32 len);
		static void Handle_C_ATTACK(GameSessionPtr session, BYTE* buffer, Shared::int32 len);

		/* Create */
		static SendBufferPtr Make_S_SpawnPlayer(const ::Protobuf::S_SpawnPlayer& pkt);
		static SendBufferPtr Make_S_SyncPlayers(const ::Protobuf::S_SyncPlayers& pkt);
	};		
}
