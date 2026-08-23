#pragma once
#include "MyGameServerLibrary/PacketHandler.h"

namespace MGSL::Net
{
	MGSL_CLASS_PTR(PacketSession)
	MGSL_CLASS_PTR(ServerSession)

	class ClientPacketHandler final : public PacketHandler<ClientPacketHandler>
	{
		MGSL_DISABLE_COPY(ClientPacketHandler)
		MGSL_DISABLE_MOVE(ClientPacketHandler)

	public:
		static void HandlePacketImpl
		(
			PacketSessionPtr session,
			BYTE* buffer, 
			Shared::int32 len
		);

	public:
		static void Handle_S_ENTER_GAME(ServerSessionPtr session, BYTE* buffer, Shared::int32 len);

	/*========================//
	//   Packet Test Methods  //
	//========================*/
	public:
		/* Packet Making */
		static SendBufferPtr Make_C_EnterGame();
		static SendBufferPtr Make_C_Move(::Protobuf::DIR_TYPE dir, bool running);
		static SendBufferPtr Make_C_Jump();
		static SendBufferPtr Make_C_ChangeWeapon(::Protobuf::WEAPON_TYPE weapon);

		/* Packet Handling */
		static void Handle_S_SPAWN(ServerSessionPtr session, BYTE* buffer, Shared::int32 len);
		static void Handle_S_SYNC_OBJECTS(ServerSessionPtr session, BYTE* buffer, Shared::int32 len);

	/*==========================//
	//   works for main thread  //
	//==========================*/
	private:
		static void SpawnObjects(const ::Protobuf::S_Spawn& pkt);
		static void ApplyEnterGame(const ::Protobuf::ObjectInfo& playerInfo);
		static void ApplySyncObjects(const ::Protobuf::S_SyncObjects& pkt);
	};
}