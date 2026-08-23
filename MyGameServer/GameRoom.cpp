#include "ServerPch.h"
#include "GameRoom.h"
#include "GameSession.h"
#include "ServerPacketHandler.h"

#include "GameObject.h"
#include "ObjectManager.h"
#include "PlayerController.h"
#include "CharacterBody2D.h"
#include "BoxCollider.h"

namespace MGSL::Net
{
	GameRoom::GameRoom() = default;
	GameRoom::~GameRoom() = default;

	void GameRoom::Init()
	{
		/*==========================================//
		//   game map's logical collider positions  //
		//==========================================*/
		// ground
		Server::GameObjectPtr ground = MGSL_OBJECT_MGR.CreateGameObject(); if (!ground) return;
		ground->GetTransform().SetPosition(Shared::vec3(7.5f, -1.45f, 0.0f));
		Server::BoxCollider* groundCollider = MGSL_OBJECT_MGR.AddComponent<Server::BoxCollider>(ground); if (!groundCollider) return;
		groundCollider->SetMobility(Server::EColliderMobility::STATIC);
		groundCollider->SetCollisionLayer(Server::ECollisionLayer::WORLD);
		groundCollider->SetSize(Shared::vec2(30.0f, 0.35f));
		MGSL_SERVER_COLLISION_MGR.Register(groundCollider);
		m_staticObjects.push_back(ground);

		// left-wall
		Server::GameObjectPtr leftWall = MGSL_OBJECT_MGR.CreateGameObject(); if (!leftWall) return;
		leftWall->GetTransform().SetPosition(Shared::vec3(-6.25f, 0.5f, 0.0f));
		Server::BoxCollider* leftWallCollider = MGSL_OBJECT_MGR.AddComponent<Server::BoxCollider>(leftWall); if (!leftWallCollider) return;
		leftWallCollider->SetMobility(Server::EColliderMobility::STATIC);
		leftWallCollider->SetCollisionLayer(Server::ECollisionLayer::WORLD);
		leftWallCollider->SetSize(Shared::vec2(0.3f, 4.0f));
		MGSL_SERVER_COLLISION_MGR.Register(leftWallCollider);
		m_staticObjects.push_back(leftWall);

		// right-wall
		Server::GameObjectPtr rightWall = MGSL_OBJECT_MGR.CreateGameObject(); if (!rightWall) return;
		rightWall->GetTransform().SetPosition(Shared::vec3(21.5f, 0.5f, 0.0f));
		Server::BoxCollider* rightWallCollider = MGSL_OBJECT_MGR.AddComponent<Server::BoxCollider>(rightWall); if (!rightWallCollider) return;
		rightWallCollider->SetMobility(Server::EColliderMobility::STATIC);
		rightWallCollider->SetCollisionLayer(Server::ECollisionLayer::WORLD);
		rightWallCollider->SetSize(Shared::vec2(0.3f, 4.0f));
		MGSL_SERVER_COLLISION_MGR.Register(rightWallCollider);
		m_staticObjects.push_back(rightWall);

		// Platform : building1
		Server::GameObjectPtr building1Roof = MGSL_OBJECT_MGR.CreateGameObject(); if (!building1Roof) return;
		building1Roof->GetTransform().SetPosition(Shared::vec3(-2.05f, -0.08f, 0.0f));
		Server::BoxCollider* building1RoofCollider = MGSL_OBJECT_MGR.AddComponent<Server::BoxCollider>(building1Roof); if (!building1RoofCollider) return;
		building1RoofCollider->SetMobility(Server::EColliderMobility::STATIC);
		building1RoofCollider->SetCollisionType(Server::ECollisionType::PLATFORM);
		building1RoofCollider->SetCollisionLayer(Server::ECollisionLayer::WORLD);
		building1RoofCollider->SetSize(Shared::vec2(2.0f, 0.125f));
		MGSL_SERVER_COLLISION_MGR.Register(building1RoofCollider);
		m_staticObjects.push_back(building1Roof);

		// Platform : building2
		Server::GameObjectPtr building2Roof = MGSL_OBJECT_MGR.CreateGameObject(); if (!building2Roof) return;
		building2Roof->GetTransform().SetPosition(Shared::vec3(1.45f, 0.35f, 0.0f));
		Server::BoxCollider* building2RoofCollider = MGSL_OBJECT_MGR.AddComponent<Server::BoxCollider>(building2Roof); if (!building2RoofCollider) return;
		building2RoofCollider->SetMobility(Server::EColliderMobility::STATIC);
		building2RoofCollider->SetCollisionType(Server::ECollisionType::PLATFORM);
		building2RoofCollider->SetCollisionLayer(Server::ECollisionLayer::WORLD);
		building2RoofCollider->SetSize(Shared::vec2(2.65f, 0.2f));
		MGSL_SERVER_COLLISION_MGR.Register(building2RoofCollider);
		m_staticObjects.push_back(building2Roof);

		// Ladder #1
		Server::GameObjectPtr ladder1 = MGSL_OBJECT_MGR.CreateGameObject(); if (!ladder1) return;
		ladder1->GetTransform().SetPosition(Shared::vec3(0.45f, -0.4f, 0.0f));
		Server::BoxCollider* ladder1Collider = MGSL_OBJECT_MGR.AddComponent<Server::BoxCollider>(ladder1); if (!ladder1Collider) return;
		ladder1Collider->SetMobility(Server::EColliderMobility::STATIC);
		ladder1Collider->SetCollisionLayer(Server::ECollisionLayer::LADDER);
		ladder1Collider->SetTrigger(true);
		ladder1Collider->SetSize(Shared::vec2(0.65f, 3.95f));
		ladder1Collider->SetOffset(Shared::vec2(-0.02f, 0.06f));
		MGSL_SERVER_COLLISION_MGR.Register(ladder1Collider);
		m_staticObjects.push_back(ladder1);

		// Platform : building3
		Server::GameObjectPtr building3Roof = MGSL_OBJECT_MGR.CreateGameObject(); if (!building3Roof) return;
		building3Roof->GetTransform().SetPosition(Shared::vec3(6.0f, 0.2f, 0.0f));
		Server::BoxCollider* building3RoofCollider = MGSL_OBJECT_MGR.AddComponent<Server::BoxCollider>(building3Roof); if (!building3RoofCollider) return;
		building3RoofCollider->SetMobility(Server::EColliderMobility::STATIC);
		building3RoofCollider->SetCollisionType(Server::ECollisionType::PLATFORM);
		building3RoofCollider->SetCollisionLayer(Server::ECollisionLayer::WORLD);
		building3RoofCollider->SetSize(Shared::vec2(1.85f, 0.185f));
		MGSL_SERVER_COLLISION_MGR.Register(building3RoofCollider);
		m_staticObjects.push_back(building3Roof);

		// Ladder #2
		Server::GameObjectPtr ladder2 = MGSL_OBJECT_MGR.CreateGameObject(); if (!ladder2) return;
		ladder2->GetTransform().SetPosition(Shared::vec3(6.5f, -0.5f, 0.0f));
		Server::BoxCollider* ladder2Collider = MGSL_OBJECT_MGR.AddComponent<Server::BoxCollider>(ladder2); if (!ladder2Collider) return;
		ladder2Collider->SetMobility(Server::EColliderMobility::STATIC);
		ladder2Collider->SetCollisionLayer(Server::ECollisionLayer::LADDER);
		ladder2Collider->SetTrigger(true);
		ladder2Collider->SetSize(Shared::vec2(0.65f, 3.95f));
		ladder2Collider->SetOffset(Shared::vec2(-0.02f, 0.06f));
		MGSL_SERVER_COLLISION_MGR.Register(ladder2Collider);
		m_staticObjects.push_back(ladder2);

		// Platform : building5
		Server::GameObjectPtr building5Roof = MGSL_OBJECT_MGR.CreateGameObject(); if (!building5Roof) return;
		building5Roof->GetTransform().SetPosition(Shared::vec3(12.75f, 0.5f, 0.0f));
		Server::BoxCollider* building5RoofCollider = MGSL_OBJECT_MGR.AddComponent<Server::BoxCollider>(building5Roof); if (!building5RoofCollider) return;
		building5RoofCollider->SetMobility(Server::EColliderMobility::STATIC);
		building5RoofCollider->SetCollisionType(Server::ECollisionType::PLATFORM);
		building5RoofCollider->SetCollisionLayer(Server::ECollisionLayer::WORLD);
		building5RoofCollider->SetSize(Shared::vec2(4.2f, 0.185f));
		MGSL_SERVER_COLLISION_MGR.Register(building5RoofCollider);
		m_staticObjects.push_back(building5Roof);

		// Ladder #3
		Server::GameObjectPtr ladder3 = MGSL_OBJECT_MGR.CreateGameObject(); if (!ladder3) return;
		ladder3->GetTransform().SetPosition(Shared::vec3(11.0f, -0.35f, 0.0f));
		Server::BoxCollider* ladder3Collider = MGSL_OBJECT_MGR.AddComponent<Server::BoxCollider>(ladder3); if (!ladder3Collider) return;
		ladder3Collider->SetMobility(Server::EColliderMobility::STATIC);
		ladder3Collider->SetCollisionLayer(Server::ECollisionLayer::LADDER);
		ladder3Collider->SetTrigger(true);
		ladder3Collider->SetSize(Shared::vec2(0.65f, 3.8f));
		ladder3Collider->SetOffset(Shared::vec2(-0.02f, 0.06f));
		MGSL_SERVER_COLLISION_MGR.Register(ladder3Collider);
		m_staticObjects.push_back(ladder3);

		// Platform : building6
		Server::GameObjectPtr building6Roof = MGSL_OBJECT_MGR.CreateGameObject(); if (!building6Roof) return;
		building6Roof->GetTransform().SetPosition(Shared::vec3(17.9f, 0.28f, 0.0f));
		Server::BoxCollider* building6RoofCollider = MGSL_OBJECT_MGR.AddComponent<Server::BoxCollider>(building6Roof); if (!building6RoofCollider) return;
		building6RoofCollider->SetMobility(Server::EColliderMobility::STATIC);
		building6RoofCollider->SetCollisionType(Server::ECollisionType::PLATFORM);
		building6RoofCollider->SetCollisionLayer(Server::ECollisionLayer::WORLD);
		building6RoofCollider->SetSize(Shared::vec2(2.8f, 0.185f));
		MGSL_SERVER_COLLISION_MGR.Register(building6RoofCollider);
		m_staticObjects.push_back(building6Roof);

		// Ladder #4
		Server::GameObjectPtr ladder4 = MGSL_OBJECT_MGR.CreateGameObject(); if (!ladder4) return;
		ladder4->GetTransform().SetPosition(Shared::vec3(17.0f, -0.45f, 0.0f));
		Server::BoxCollider* ladder4Collider = MGSL_OBJECT_MGR.AddComponent<Server::BoxCollider>(ladder4); if (!ladder4Collider) return;
		ladder4Collider->SetMobility(Server::EColliderMobility::STATIC);
		ladder4Collider->SetCollisionLayer(Server::ECollisionLayer::LADDER);
		ladder4Collider->SetTrigger(true);
		ladder4Collider->SetSize(Shared::vec2(0.65f, 3.95f));
		ladder4Collider->SetOffset(Shared::vec2(-0.02f, 0.06f));
		MGSL_SERVER_COLLISION_MGR.Register(ladder4Collider);
		m_staticObjects.push_back(ladder4);
	}

	void GameRoom::Update(float deltaTime)
	{
		// 1. Update
		for (auto& [objectID, player] : m_players)
		{
			if (!player) continue;
			player->Update(deltaTime);
		}

		// 2. 이동 결과를 기준으로 충돌 검사 및 위치 보정
		MGSL_SERVER_COLLISION_MGR.Update();

		// 3. LateUpdate
		for (auto& [objectID, player] : m_players)
		{
			if (!player) continue;
			player->LateUpdate(deltaTime);
		}

		// 4. info 동기화
		for (auto& [objectID, player] : m_players)
		{
			if (!player) continue;

			Server::CharacterBody2D* body = player->GetComponent<Server::CharacterBody2D>();
			if (!body) continue;

			Server::PlayerController* controller = player->GetComponent<Server::PlayerController>();
			if (!controller) continue;

			const auto& position = player->GetTransform().GetPosition();
			auto& info = player->GetObjectInfo();
			info.set_posx(position.x);
			info.set_posy(position.y);
			info.set_velocityx(body->GetHorizontalVelocity());
			info.set_velocityy(body->GetVerticalVelocity());
			info.set_grounded(body->IsGrounded());
			info.set_state(controller->GetState());
			info.set_facing(controller->GetFacing());
			info.set_weapon(controller->GetWeapon());
		}

		// TEMP : 5. 해당 Tick의 상태를 한 번에 전송
		BroadcastSyncObjects();
	}

	void GameRoom::EnterGameRoom(GameSessionPtr session)
	{
		// 0. 세션 검증 및 중복 입장 방지
		if (!session) return;
		if (session->GetGameObject()) return;

		// 1. 서버 논리 플레이어 생성
		Server::GameObjectPtr player = MGSL_OBJECT_MGR.CreateGameObject();
		if (!player) return;
		player->GetTransform().SetScale(Shared::vec3(1.25f));

		// 2. 서버 전용 컴포넌트 부착
		MGSL_OBJECT_MGR.AddComponent<Server::PlayerController>(player);
		MGSL_OBJECT_MGR.AddComponent<Server::CharacterBody2D>(player);
		auto* playerCollider = MGSL_OBJECT_MGR.AddComponent<Server::BoxCollider>(player);
		playerCollider->SetMobility(Server::EColliderMobility::DYNAMIC);
		playerCollider->SetCollisionLayer(Server::ECollisionLayer::PLAYER);
		playerCollider->SetSize(Shared::vec2(0.3f, 0.6f));
		playerCollider->SetOffset(Shared::vec2(0.0f, -0.2f));
		MGSL_SERVER_COLLISION_MGR.Register(playerCollider);

		// 3. GameObject <-> Room / Session 연결
		player->SetGameRoom(GetGameRoom());
		player->SetGameSession(session);
		session->SetGameObject(player);

		// 4. 기존 플레이어들을 신규 플레이어에게 전달
		SendExistingPlayers(session);

		// 5. Room 등록
		const Shared::uint64 objectID = player->GetObjectInfo().objectid();
		m_players.emplace(objectID, player);

		// 6. 신규 플레이어에게 자신의 입장 완료 정보 전달, S_EnterGame 전송
		session->GetSessionBuffer().Send(ServerPacketHandler::Create_S_EnterGame(true, player->GetObjectInfo()));

		// 7. 기존 플레이어들에게 신규 플레이어 전달
		BroadcastSpawn(player);

		MGSL_LOG_INFO
		(
			"Player entered GameRoom. ObjectID = {}",
			objectID
		);
	}

	void GameRoom::LeaveGameRoom(GameSessionPtr session)
	{

	}

	void GameRoom::SendExistingPlayers(GameSessionPtr session)
	{
		if (!session) return;

		MGSL_LOG_INFO
		(
			"SendExistingPlayers called. PlayerCount = {}",
			m_players.size()
		);

		Protobuf::S_Spawn spawnPkt;
		for (const auto& [id, otherPlayer] : m_players)
		{
			if (!otherPlayer) continue;
			*spawnPkt.add_objects() = otherPlayer->GetObjectInfo();
		}

		if (spawnPkt.objects_size() == 0) return;
		MGSL_LOG_INFO("Send S_Spawn. ObjectCount = {}", spawnPkt.objects_size());
		session->GetSessionBuffer().Send(ServerPacketHandler::Make_S_Spawn(spawnPkt));
	}

	void GameRoom::BroadcastSpawn(Server::GameObjectPtr player)
	{
		if (!player) return;

		Protobuf::S_Spawn spawnPkt;
		*spawnPkt.add_objects() = player->GetObjectInfo();

		const Shared::uint64 newObjectID = player->GetObjectInfo().objectid();
		for (const auto& [objectID, otherPlayer] : m_players)
		{
			if (!otherPlayer) continue;
			if (objectID == newObjectID) continue;

			GameSessionPtr otherSession = otherPlayer->GetGameSession();
			if (!otherSession) continue;
			otherSession->GetSessionBuffer().Send(ServerPacketHandler::Make_S_Spawn(spawnPkt));
		}
	}

	//void GameRoom::BroadcastMove(Server::GameObjectPtr player)
	//{
	//	if (!player) return;

	//	Protobuf::S_Move movePkt;
	//	*movePkt.mutable_object() = player->GetObjectInfo();

	//	const auto objectID =player->GetObjectInfo().objectid();
	//	for (const auto& [id, otherPlayer] : m_players)
	//	{
	//		if (!otherPlayer) continue;

	//		auto session = otherPlayer->GetGameSession();
	//		if (!session) continue;

	//		session->GetSessionBuffer().Send(ServerPacketHandler::Make_S_Move(movePkt));
	//	}
	//}

	void GameRoom::BroadcastSyncObjects()
	{
		::Protobuf::S_SyncObjects pkt;

		for (const auto& [objectID, player] : m_players)
		{
			if (!player) continue;
			*pkt.add_objects() = player->GetObjectInfo();
		}

		if (pkt.objects_size() == 0) return;
		auto sendBuffer = ServerPacketHandler::Make_S_SyncObjects(pkt);

		for (const auto& [objectID, player] : m_players)
		{
			if (!player) continue;
			auto session = player->GetGameSession();
			if (!session) continue;
			session->GetSessionBuffer().Send(sendBuffer);
		}
	}

	GameRoomPtr GameRoom::GetGameRoom()
	{
		return shared_from_this();
	}
}
