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
		static SendBufferPtr Make_C_Move(::Protobuf::DIR_TYPE horizontalDir, ::Protobuf::DIR_TYPE verticalDir, bool running);
		static SendBufferPtr Make_C_Jump();
		static SendBufferPtr Make_C_ChangeWeapon(::Protobuf::WEAPON_TYPE weapon);
		static SendBufferPtr Make_C_Attack();

		/* Packet Handling */
		static void Handle_S_SPAWN_PLAYER(ServerSessionPtr session, BYTE* buffer, Shared::int32 len);
		static void Handle_S_SYNC_PLAYERS(ServerSessionPtr session, BYTE* buffer, Shared::int32 len);
		static void Handle_S_REMOVE_PLAYER(ServerSessionPtr session, BYTE* buffer, Shared::int32 len);
		static void Handle_S_SPAWN_BULLET(ServerSessionPtr session, BYTE* buffer, Shared::int32 len);
		static void Handle_S_SYNC_BULLETS(ServerSessionPtr session, BYTE* buffer, Shared::int32 len);
		static void Handle_S_REMOVE_BULLET(ServerSessionPtr session, BYTE* buffer, Shared::int32 len);
		static void Handle_S_SPAWN_EFFECT(ServerSessionPtr session, BYTE* buffer, Shared::int32 len);

	/*==========================//
	//   works for main thread  //
	//==========================*/
	private:
		static void ApplyEnterGame(const ::Protobuf::PlayerInfo& playerInfo);
		static void SpawnPlayer(const ::Protobuf::S_SpawnPlayer& pkt);
		static void ApplySyncPlayers(const ::Protobuf::S_SyncPlayers& pkt);
		static void RemovePlayer(const ::Protobuf::S_RemovePlayer& pkt);
		static void SpawnBullet(const ::Protobuf::S_SpawnBullet& pkt);
		static void ApplySyncBullets(const ::Protobuf::S_SyncBullets& pkt);
		static void RemoveBullet(const ::Protobuf::S_RemoveBullet& pkt);
		static void SpawnEffect(const ::Protobuf::S_SpawnEffect& pkt);
	};
}
