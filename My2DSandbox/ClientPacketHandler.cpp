#include "2DPch.h"
#include "ClientPacketHandler.h"
#include "Sandbox2D.h" 
#include "ServerSession.h"
#include "MyGameServerProtocol/PacketHeader.h"
#include "MyGameServerLibrary/BufferReader.h"
#include "MyGameFramework/Scene.h"
#include "MyGameFramework/Texture2D.h"
#include "MyGameFramework/FlipbookClip.h"
#include "MyGameFramework/FlipbookController.h"
#include "MyGameFramework/FlipbookPlayer.h"
#include "MyGameFramework/BoxCollider.h"
#include "MyGameFramework/CharacterBody2D.h"
#include "MyPlayerController.h"
#include "Sandbox2DGlobal.h"

namespace MGSL::Net
{
	void ClientPacketHandler::HandlePacketImpl
	(
		PacketSessionPtr session,
		BYTE* buffer,
		Shared::int32 len
	)
	{
		Net::BufferReader br(buffer, len);

		Protocol::PacketHeader header;
		br >> header;

		// 똑같이 패킷 아이디를 읽어서 분기처리
		auto serverSession = std::static_pointer_cast<ServerSession>(session);
		switch (static_cast<Protocol::PacketID>(header.id))
		{
		case Protocol::PacketID::S_EnterGame:
			Handle_S_ENTER_GAME(serverSession, buffer, len);
			break;

		case Protocol::PacketID::S_Spawn:
			Handle_S_SPAWN(serverSession, buffer, len);
			break;

		case Protocol::PacketID::S_SyncObjects:
			Handle_S_SYNC_OBJECTS(serverSession, buffer, len);
			break;
		}
	}

	void ClientPacketHandler::Handle_S_ENTER_GAME(ServerSessionPtr session, BYTE* buffer, Shared::int32 len)
	{
		Protocol::PacketHeader* header = reinterpret_cast<Protocol::PacketHeader*>(buffer);
		Shared::uint16 id = header->id;
		Shared::uint16 size = header->size;

		::Protobuf::S_EnterGame pkt;
		pkt.ParseFromArray(&header[1], size - sizeof(Protocol::PacketHeader));
		if (!pkt.success()) return;

		const ::Protobuf::ObjectInfo& playerInfo = pkt.player();
		MGSL_NETWORK_MGR.RunOnMainThread([playerInfo = std::move(playerInfo)](){ ApplyEnterGame(playerInfo); });
	}

	/*========================//
	//   Packet Test Methods  //
	//========================*/
	SendBufferPtr ClientPacketHandler::Make_C_EnterGame()
	{
		::Protobuf::C_EnterGame pkt;
		return MakeSendBuffer(pkt, static_cast<Shared::uint16>(Protocol::PacketID::C_EnterGame));
	}

	SendBufferPtr ClientPacketHandler::Make_C_Move(::Protobuf::DIR_TYPE dir)
	{
		::Protobuf::C_Move pkt;
		pkt.set_dir(dir);
		return MakeSendBuffer(pkt, static_cast<Shared::uint16>(Protocol::PacketID::C_Move));
	}

	SendBufferPtr ClientPacketHandler::Make_C_Jump()
	{
		::Protobuf::C_Jump pkt;
		return MakeSendBuffer(pkt, static_cast<Shared::uint16>(Protocol::PacketID::C_Jump));
	}

	void ClientPacketHandler::Handle_S_SPAWN(ServerSessionPtr session, BYTE* buffer, Shared::int32 len)
	{
		Protocol::PacketHeader* header = reinterpret_cast<Protocol::PacketHeader*>(buffer);
		const Shared::uint16 size = header->size;

		::Protobuf::S_Spawn pkt;
		pkt.ParseFromArray(&header[1], size - sizeof(Protocol::PacketHeader));

		MGSL_NETWORK_MGR.RunOnMainThread([pkt = std::move(pkt)]() { SpawnObjects(std::move(pkt)); });
	}

	void ClientPacketHandler::Handle_S_SYNC_OBJECTS(ServerSessionPtr session, BYTE* buffer, Shared::int32 len)
	{
		auto* header = reinterpret_cast<Protocol::PacketHeader*>(buffer);
		const Shared::uint16 size = header->size;

		::Protobuf::S_SyncObjects pkt;
		pkt.ParseFromArray(&header[1], size - sizeof(Protocol::PacketHeader));

		MGSL_NETWORK_MGR.RunOnMainThread([pkt = std::move(pkt)]() { ApplySyncObjects(pkt); });
	}

	/*==========================//
	//   works for main thread  //
	//==========================*/
	void ClientPacketHandler::SpawnObjects(const ::Protobuf::S_Spawn& pkt)
	{
		Framework::Scene* scene = g_Game2D.GetScene();
		if (!scene) return;

		Framework::Texture2DPtr playerTexture = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Fighter.Idle");
		if (!playerTexture) return;

		for (const auto& objectInfo : pkt.objects())
		{
			// 1. 다른 플레이어용 GameObject 생성
			auto otherPlayer = MGSL_OBJECT_MGR.CreateGameObject(scene);
			if (!otherPlayer) continue;

			// 2. 서버에서 전달받은 네트워크 정보 설정
			otherPlayer->SetObjectInfo(objectInfo);

			// 3. 이후 컴포넌트 부착
			Framework::FlipbookControllerPtr flipbookController = Framework::FlipbookController::Create(playerTexture); if (!flipbookController) continue;
			Shared::List<Shared::vec4> idleFrames =
			{
				{ 0.000f, 0.0f, 0.125f, 1.0f },
				{ 0.125f, 0.0f, 0.125f, 1.0f },
				{ 0.250f, 0.0f, 0.125f, 1.0f },
				{ 0.375f, 0.0f, 0.125f, 1.0f },
				{ 0.500f, 0.0f, 0.125f, 1.0f },
				{ 0.625f, 0.0f, 0.125f, 1.0f },
				{ 0.750f, 0.0f, 0.125f, 1.0f },
				{ 0.875f, 0.0f, 0.125f, 1.0f }
			};
			Framework::FlipbookClipPtr idleClip = Framework::FlipbookClip::Create(idleFrames, 16.0f); if (!idleClip) continue;
			if (!flipbookController->SetClip(0, idleClip)) continue;
			Framework::FlipbookPlayer* flipbookPlayer = MGSL_OBJECT_MGR.AddComponent<Framework::FlipbookPlayer>(otherPlayer, flipbookController); if (!flipbookPlayer) continue;
			if (!flipbookPlayer->SetState(0)) continue;
			flipbookPlayer->SetSize(Shared::vec2(1.0f, 1.0f));
			flipbookPlayer->Play();

			// 4. BoxCollider 부착
			Framework::BoxCollider* collider = MGSL_OBJECT_MGR.AddComponent<Framework::BoxCollider>(otherPlayer);
			if (!collider) continue;
			collider->SetDebugSize(Shared::vec2(0.3f, 0.6f));
			collider->SetDebugOffset(Shared::vec2(0.0f, -0.2f));
			MGSL_COLLIDE_MGR.Register(collider);

			// 5. CharacterBody2D 부착
			Framework::CharacterBody2D* body = MGSL_OBJECT_MGR.AddComponent<Framework::CharacterBody2D>(otherPlayer);
			if (!body) continue;

			// 6. 최초 위치 설정
			Shared::vec3 spawnPosition
			(
				objectInfo.posx(),
				objectInfo.posy(),
				0.0f
			);
			otherPlayer->GetTransform().SetPosition(spawnPosition);
			otherPlayer->GetTransform().SetScale(Shared::vec3(1.25f));
		}
	}

	void ClientPacketHandler::ApplyEnterGame(const ::Protobuf::ObjectInfo& playerInfo)
	{
		Framework::Scene* scene = g_Game2D.GetScene();
		if (!scene) return;

		Framework::GameObject* myPlayer = MGSL_OBJECT_MGR.FindGameObjectWithComponent<Sandbox2D::MyPlayerController>(scene);
		if (!myPlayer) return;

		myPlayer->SetObjectInfo(playerInfo);
	}

	void ClientPacketHandler::ApplySyncObjects(const ::Protobuf::S_SyncObjects& pkt)
	{
		Framework::Scene* scene = g_Game2D.GetScene();
		if (!scene) return;

		for (const auto& objectInfo : pkt.objects())
		{
			Framework::GameObject* gameObject = MGSL_OBJECT_MGR.FindGameObject(scene, objectInfo.objectid());
			if (!gameObject) continue;

			// 위치 동기화
			Shared::vec3 serverPosition(objectInfo.posx(), objectInfo.posy(), 0.0f);

			// 충돌 동기화
			Framework::BoxCollider* collider = gameObject->GetComponent<Framework::BoxCollider>();
			if (collider) collider->SetServerPosition(serverPosition);

			// 물리 동기화
			Framework::CharacterBody2D* body = gameObject->GetComponent<Framework::CharacterBody2D>();
			if (body)
			{
				body->SetServerVelocity(objectInfo.velocityx(), objectInfo.velocityy());
				body->SetServerGrounded(objectInfo.grounded());
			}
		}
	}
}
