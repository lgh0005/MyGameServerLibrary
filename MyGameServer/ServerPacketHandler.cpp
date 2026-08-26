#include "ServerPch.h"
#include "Server.h"
#include "GameRoom.h"
#include "GameSession.h"
#include "GameObject.h"
#include "CharacterBody2D.h"
#include "ServerPacketHandler.h"
#include "MyGameServerProtocol/PacketHeader.h"
#include "MyGameServerLibrary/BufferReader.h"
#include "PlayerController.h"

namespace MGSL::Net
{
	void ServerPacketHandler::HandlePacketImpl
	(
		PacketSessionPtr session,
		BYTE* buffer, 
		Shared::int32 len
	)
	{
		BufferReader br(buffer, len);

		Protocol::PacketHeader header;
		br.Peek(&header);

		auto gameSession = std::static_pointer_cast<GameSession>(session);
		switch (static_cast<EPacketID>(header.id))
		{
		case EPacketID::C_EnterGame:
			Handle_C_ENTER_GAME(gameSession, buffer, len);
			break;

		case EPacketID::C_Move:
			Handle_C_MOVE(gameSession, buffer, len);
			break;

		case EPacketID::C_Jump:
			Handle_C_JUMP(gameSession, buffer, len);
			break;

		case EPacketID::C_ChangeWeapon:
			Handle_C_CHANGE_WEAPON(gameSession, buffer, len);
			break;

		case EPacketID::C_Attack:
			Handle_C_ATTACK(gameSession, buffer, len);
			break;

		default:
			break;
		}
	}

	/*=================================//
	//    default packet generators    //
	//=================================*/
	SendBufferPtr ServerPacketHandler::Create_S_EnterGame(bool success, const Protobuf::PlayerInfo& player)
	{
		// Protobuf를 이용한 패킷 생성
		::Protobuf::S_EnterGame pkt;

		pkt.set_success(success);
		*pkt.mutable_player() = player;

		return MakeSendBuffer(pkt, static_cast<Shared::uint16>(Protocol::PacketID::S_EnterGame));
	}

	/*========================//
	//   Packet Test Methods  //
	//========================*/
	void ServerPacketHandler::Handle_C_MOVE(GameSessionPtr session, BYTE* buffer, Shared::int32 len)
	{
		if (!session) return;
		Protocol::PacketHeader* header = reinterpret_cast<Protocol::PacketHeader*>(buffer);
		
		::Protobuf::C_Move pkt;
		pkt.ParseFromArray(&header[1], header->size - sizeof(Protocol::PacketHeader));

		Server::GameObject* player = session->GetGameObject();
		if (!player) return;
		
		auto* playerController = player->GetComponent<Server::PlayerController>();
		if (!playerController) return;

		const auto horizontalDir = pkt.horizontal_dir();
		const auto verticalDir = pkt.vertical_dir();
		const bool running = pkt.running();
		playerController->SetHorizontalDirection(horizontalDir);
		playerController->SetVerticalDirection(verticalDir);
		playerController->SetRunning(running);

		// DEBUF
		MGSL_LOG_INFO
		(
			"C_Move received. ObjectID = {}, Horizontal = {}, Vertical = {}, Running = {}",
			playerController->GetObjectID(),
			static_cast<int>(horizontalDir),
			static_cast<int>(verticalDir),
			running
		);
	}

	void ServerPacketHandler::Handle_C_ENTER_GAME(GameSessionPtr session, BYTE* buffer, Shared::int32 len)
	{
		if (!session) return;

		auto room = g_Server.GetGameRoom();
		if (!room) return;

		room->EnterGameRoom(session);
	}

	void ServerPacketHandler::Handle_C_JUMP(GameSessionPtr session, BYTE* buffer, Shared::int32 len)
	{
		if (!session) return;

		Server::GameObject* player = session->GetGameObject();
		if (!player) return;

		Server::PlayerController* playerController = player->GetComponent<Server::PlayerController>();
		if (!playerController) return;

		auto* body = player->GetComponent<Server::CharacterBody2D>();
		if (!body) return;

		body->Jump(3.5f);

		MGSL_LOG_INFO
		(
			"C_Jump received. ObjectID = {}",
			playerController->GetObjectID()
		);
	}

	void ServerPacketHandler::Handle_C_CHANGE_WEAPON(GameSessionPtr session, BYTE* buffer, Shared::int32 len)
	{
		if (!session) return;
		Protocol::PacketHeader* header = reinterpret_cast<Protocol::PacketHeader*>(buffer);

		Protobuf::C_ChangeWeapon pkt;
		if (!pkt.ParseFromArray(&header[1], header->size - sizeof(Protocol::PacketHeader)))
			return;

		Server::GameObject* player = session->GetGameObject();
		if (!player) return;

		Server::PlayerController* playerController = player->GetComponent<Server::PlayerController>();
		if (!playerController) return;

		playerController->SetWeapon(pkt.weapon());
	}

	void ServerPacketHandler::Handle_C_ATTACK(GameSessionPtr session, BYTE* buffer, Shared::int32 len)
	{
		if (!session) return;
		Protocol::PacketHeader* header = reinterpret_cast<Protocol::PacketHeader*>(buffer);

		Protobuf::C_Attack pkt;
		if (!pkt.ParseFromArray(&header[1], header->size - sizeof(Protocol::PacketHeader)))
			return;

		Server::GameObject* player = session->GetGameObject();
		if (!player) return;

		Server::PlayerController* playerController = player->GetComponent<Server::PlayerController>();
		if (!playerController) return;

		playerController->Attack();
	}

	SendBufferPtr ServerPacketHandler::Make_S_SpawnPlayer(const ::Protobuf::S_SpawnPlayer& pkt)
	{
		return MakeSendBuffer(pkt, static_cast<Shared::uint16>(Protocol::PacketID::S_SpawnPlayer));
	}

	SendBufferPtr ServerPacketHandler::Make_S_SyncPlayers(const ::Protobuf::S_SyncPlayers& pkt)
	{
		return MakeSendBuffer(pkt, static_cast<Shared::uint16>(Protocol::PacketID::S_SyncPlayers));
	}
}
