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
		groundCollider->SetSize(Shared::vec2(30.0f, 0.35f));
		MGSL_SERVER_COLLISION_MGR.Register(groundCollider);
		m_staticObjects.push_back(ground);

		// left-wall
		Server::GameObjectPtr leftWall = MGSL_OBJECT_MGR.CreateGameObject(); if (!leftWall) return;
		leftWall->GetTransform().SetPosition(Shared::vec3(-6.25f, 0.5f, 0.0f));
		Server::BoxCollider* leftWallCollider = MGSL_OBJECT_MGR.AddComponent<Server::BoxCollider>(leftWall); if (!leftWallCollider) return;
		leftWallCollider->SetMobility(Server::EColliderMobility::STATIC);
		leftWallCollider->SetSize(Shared::vec2(0.3f, 4.0f));
		MGSL_SERVER_COLLISION_MGR.Register(leftWallCollider);
		m_staticObjects.push_back(leftWall);

		// right-wall
		Server::GameObjectPtr rightWall = MGSL_OBJECT_MGR.CreateGameObject(); if (!rightWall) return;
		rightWall->GetTransform().SetPosition(Shared::vec3(21.5f, 0.5f, 0.0f));
		Server::BoxCollider* rightWallCollider = MGSL_OBJECT_MGR.AddComponent<Server::BoxCollider>(rightWall); if (!rightWallCollider) return;
		rightWallCollider->SetMobility(Server::EColliderMobility::STATIC);
		rightWallCollider->SetSize(Shared::vec2(0.3f, 4.0f));
		MGSL_SERVER_COLLISION_MGR.Register(rightWallCollider);
		m_staticObjects.push_back(rightWall);
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

			const auto& position = player->GetTransform().GetPosition();
			auto& info = player->GetObjectInfo();
			info.set_posx(position.x);
			info.set_posy(position.y);
			info.set_velocityx(body->GetHorizontalVelocity());
			info.set_velocityy(body->GetVerticalVelocity());
			info.set_grounded(body->IsGrounded());
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