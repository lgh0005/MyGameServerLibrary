#include "2DPch.h"
#include "ClientPacketHandler.h"
#include "Sandbox2D.h" 
#include "ServerSession.h"
#include "MyGameServerProtocol/PacketHeader.h"
#include "MyGameServerLibrary/BufferReader.h"
#include "MyGameFramework/Scene.h"
#include "MyGameFramework/Texture2D.h"
#include "MyGameFramework/FlipbookController.h"
#include "MyGameFramework/FlipbookPlayer.h"
#include "MyGameFramework/BoxCollider.h"
#include "MyGameFramework/CharacterBody2D.h"
#include "MyPlayerNetworkState.h"
#include "MyPlayerController.h"
#include "MyPlayerStateMachine.h"
#include "Sandbox2DGlobal.h"
#include "FlipbookUtils.h"

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

		case Protocol::PacketID::S_SpawnPlayer:
			Handle_S_SPAWN_PLAYER(serverSession, buffer, len);
			break;

		case Protocol::PacketID::S_SyncPlayers:
			Handle_S_SYNC_PLAYERS(serverSession, buffer, len);
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

		const ::Protobuf::PlayerInfo& playerInfo = pkt.player();
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

	SendBufferPtr ClientPacketHandler::Make_C_Move(::Protobuf::DIR_TYPE horizontalDir, ::Protobuf::DIR_TYPE verticalDir, bool running)
	{
		::Protobuf::C_Move pkt;
		pkt.set_horizontal_dir(horizontalDir);
		pkt.set_vertical_dir(verticalDir);
		pkt.set_running(running);
		return MakeSendBuffer(pkt, static_cast<Shared::uint16>(Protocol::PacketID::C_Move));
	}

	SendBufferPtr ClientPacketHandler::Make_C_Jump()
	{
		::Protobuf::C_Jump pkt;
		return MakeSendBuffer(pkt, static_cast<Shared::uint16>(Protocol::PacketID::C_Jump));
	}

	SendBufferPtr ClientPacketHandler::Make_C_ChangeWeapon(::Protobuf::WEAPON_TYPE weapon)
	{
		::Protobuf::C_ChangeWeapon pkt;
		pkt.set_weapon(weapon);
		return MakeSendBuffer(pkt, static_cast<Shared::uint16>(Protocol::PacketID::C_ChangeWeapon));
	}

	SendBufferPtr ClientPacketHandler::Make_C_Attack()
	{
		::Protobuf::C_Attack pkt;
		return MakeSendBuffer(pkt, static_cast<Shared::uint16>(Protocol::PacketID::C_Attack));
	}

	void ClientPacketHandler::Handle_S_SPAWN_PLAYER(ServerSessionPtr session, BYTE* buffer, Shared::int32 len)
	{
		Protocol::PacketHeader* header = reinterpret_cast<Protocol::PacketHeader*>(buffer);
		const Shared::uint16 size = header->size;

		::Protobuf::S_SpawnPlayer pkt;
		pkt.ParseFromArray(&header[1], size - sizeof(Protocol::PacketHeader));

		MGSL_NETWORK_MGR.RunOnMainThread([pkt = std::move(pkt)]() { SpawnPlayers(std::move(pkt)); });
	}

	void ClientPacketHandler::Handle_S_SYNC_PLAYERS(ServerSessionPtr session, BYTE* buffer, Shared::int32 len)
	{
		auto* header = reinterpret_cast<Protocol::PacketHeader*>(buffer);
		const Shared::uint16 size = header->size;

		::Protobuf::S_SyncPlayers pkt;
		pkt.ParseFromArray(&header[1], size - sizeof(Protocol::PacketHeader));

		MGSL_NETWORK_MGR.RunOnMainThread([pkt = std::move(pkt)]() { ApplySyncPlayers(pkt); });
	}

	/*==========================//
	//   works for main thread  //
	//==========================*/
	void ClientPacketHandler::SpawnPlayers(const ::Protobuf::S_SpawnPlayer& pkt)
	{
		Framework::Scene* scene = g_Game2D.GetScene();
		if (!scene) return;

		Framework::Texture2DPtr fighterAtlas = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Fighter"); if (!fighterAtlas) return;
		Framework::Texture2DPtr pistolAtlas = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Pistol"); if (!pistolAtlas) return;
		Framework::Texture2DPtr swordAtlas = MGSL_RESOURCE_MGR.GetResource<Framework::Texture2D>("Sandbox2D.Player.Sword"); if (!swordAtlas) return;

		for (const auto& objectInfo : pkt.objects())
		{
			// 1. 다른 플레이어용 GameObject 생성
			auto otherPlayer = MGSL_OBJECT_MGR.CreateGameObject(scene);
			if (!otherPlayer) continue;

			// 2. NetworkState 부착 및 최초 정보 적용
			Sandbox2D::MyPlayerNetworkState* networkState = MGSL_OBJECT_MGR.AddComponent<Sandbox2D::MyPlayerNetworkState>(otherPlayer);
			networkState->Apply(objectInfo);

			// 3. Flipbook Controller 생성
			Framework::FlipbookControllerPtr fighterController = Sandbox2D::FlipbookUtils::MakeFighterController(fighterAtlas); if (!fighterController) continue;
			Framework::FlipbookControllerPtr pistolController = Sandbox2D::FlipbookUtils::MakePistolController(pistolAtlas); if (!pistolController) continue;
			Framework::FlipbookControllerPtr swordController = Sandbox2D::FlipbookUtils::MakeSwordController(swordAtlas); if (!swordController) continue;

			// 4. FlipbookPlayer 부착
			Framework::FlipbookPlayer* flipbookPlayer = MGSL_OBJECT_MGR.AddComponent<Framework::FlipbookPlayer>(otherPlayer); if (!flipbookPlayer) continue;
			flipbookPlayer->ResizeControllers(static_cast<Shared::usize>(Sandbox2D::EWeaponType::COUNT));
			if (!flipbookPlayer->SetController((Shared::usize)Sandbox2D::EWeaponType::FIGHTER, fighterController)) continue;
			if (!flipbookPlayer->SetController((Shared::usize)Sandbox2D::EWeaponType::PISTOL, pistolController)) continue;
			if (!flipbookPlayer->SetController((Shared::usize)Sandbox2D::EWeaponType::SWORD, swordController)) continue;
			if (!flipbookPlayer->ChangeController((Shared::usize)Sandbox2D::EWeaponType::FIGHTER)) continue;
			if (!flipbookPlayer->SetState(static_cast<Shared::uint32>(Sandbox2D::EObjectState::IDLE))) continue;
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
			body->SetGravity(-11.0f);

			// 6. MyPlayerStateMachine 부착
			Sandbox2D::MyPlayerStateMachine* stateMachine = MGSL_OBJECT_MGR.AddComponent<Sandbox2D::MyPlayerStateMachine>(otherPlayer);
			if (!stateMachine) continue;

			// 7. 최초 위치 설정
			Shared::vec3 spawnPosition(objectInfo.position().x(), objectInfo.position().y(), 0.0f);
			otherPlayer->GetTransform().SetPosition(spawnPosition);
			otherPlayer->GetTransform().SetScale(Shared::vec3(1.25f));

			// 8. 네트워크 오브젝트 등록
			MGSL_OBJECT_MGR.RegisterNetworkObject(scene, objectInfo.objectid(), otherPlayer);
		}
	}

	void ClientPacketHandler::ApplyEnterGame(const ::Protobuf::PlayerInfo& playerInfo)
	{
		Framework::Scene* scene = g_Game2D.GetScene();
		if (!scene) return;

		Framework::GameObject* myPlayer = MGSL_OBJECT_MGR.FindGameObjectWithComponent<Sandbox2D::MyPlayerController>(scene);
		if (!myPlayer) return;

		Sandbox2D::MyPlayerNetworkState* networkState = myPlayer->GetComponent<Sandbox2D::MyPlayerNetworkState>();
		if (!networkState) return;
		networkState->Apply(playerInfo);

		MGSL_OBJECT_MGR.RegisterNetworkObject(scene, playerInfo.objectid(), myPlayer);
	}

	void ClientPacketHandler::ApplySyncPlayers(const ::Protobuf::S_SyncPlayers& pkt)
	{
		Framework::Scene* scene = g_Game2D.GetScene();
		if (!scene) return;

		for (const auto& objectInfo : pkt.objects())
		{
			Framework::GameObject* gameObject = MGSL_OBJECT_MGR.FindNetworkObject(scene, objectInfo.objectid());
			if (!gameObject) continue;

			Sandbox2D::MyPlayerNetworkState* networkState = gameObject->GetComponent<Sandbox2D::MyPlayerNetworkState>();
			if (networkState) networkState->Apply(objectInfo);

			// 위치 동기화
			const Shared::vec3 serverPosition { objectInfo.position().x(), objectInfo.position().y(), 0.0f };

			// 충돌 동기화
			Framework::BoxCollider* collider = gameObject->GetComponent<Framework::BoxCollider>();
			if (collider) collider->SetServerPosition(serverPosition);

			// 물리 동기화
			Framework::CharacterBody2D* body = gameObject->GetComponent<Framework::CharacterBody2D>();
			if (body)
			{
				body->SetServerVelocity(objectInfo.velocity().x(), objectInfo.velocity().y());
				body->SetServerGrounded(objectInfo.grounded());
			}
		}
	}
}
