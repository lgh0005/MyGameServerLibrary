#include "ServerPch.h"
#include "GameRoom.h"
#include "GameSession.h"
#include "ServerPacketHandler.h"
#include "ColorUtils.h"

#include "VirtualSandbox2DScene.h"
#include "GameObject.h"
#include "PlayerController.h"
#include "CharacterBody2D.h"
#include "BoxCollider.h"

namespace MGSL::Net
{
	GameRoom::GameRoom() = default;
	GameRoom::~GameRoom() = default;

	void GameRoom::Init()
	{
		// create virtual scene of "Sandbox2DScene"
		m_virtualScene = std::make_unique<Server::VirtualSandbox2DScene>();
		m_virtualScene->Init();
	}

	void GameRoom::Update(float deltaTime)
	{
		if (!m_virtualScene) return;

		/*========================//
		//      Scene Update      //
		//========================*/
		m_virtualScene->Update(deltaTime);

		/*========================//
		//    ObjectInfo Sync     //
		//========================*/
		for (auto& [objectID, controller] : m_players)
		{
			if (!controller) continue;

			Server::GameObject* player = controller->GetOwner();
			if (!player) continue;

			Server::CharacterBody2D* body = player->GetComponent<Server::CharacterBody2D>();
			if (!body) continue;

			auto& info = controller->GetInfo();
			const auto& position = player->GetTransform().GetPosition();
			info.mutable_position()->set_x(position.x);
			info.mutable_position()->set_y(position.y);
			info.mutable_velocity()->set_x(body->GetHorizontalVelocity());
			info.mutable_velocity()->set_y(body->GetVerticalVelocity());
			info.set_grounded(body->IsGrounded());
		}

		/*========================//
		//      Network Sync      //
		//========================*/
		BroadcastSyncObjects();
	}

	void GameRoom::EnterGameRoom(GameSessionPtr session)
	{
		// 0. 세션 검증 및 중복 입장 방지
		if (!session) return;
		if (!m_virtualScene) return;
		if (session->GetGameObject()) return;
		auto scenePtr = m_virtualScene.get();

		// 1. 서버 논리 플레이어 생성
		Server::GameObject* player = MGSL_OBJECT_MGR.CreateGameObject(scenePtr); if (!player) return;
		player->GetTransform().SetScale(Shared::vec3(1.25f));
		Server::PlayerController* controller = MGSL_OBJECT_MGR.AddComponent<Server::PlayerController>(player); if (!controller) return;
		Server::CharacterBody2D* body = MGSL_OBJECT_MGR.AddComponent<Server::CharacterBody2D>(player); if (!body) return;
		body->SetGravity(-11.0f);
		Server::BoxCollider* playerCollider = MGSL_OBJECT_MGR.AddComponent<Server::BoxCollider>(player); if (!playerCollider) return;

		controller->SetObjectID(MGSL_OBJECT_MGR.GenerateNetworkObjectID());
		const Shared::vec3 playerColor = Server::ColorUtils::HSVToRGB(Shared::Random::Range(0.0f, 360.0f), 0.75f, 1.0f);
		Protobuf::Color* color = controller->GetInfo().mutable_color();
		color->set_r(playerColor.r);
		color->set_g(playerColor.g);
		color->set_b(playerColor.b);
		color->set_a(1.0f);

		// 2. 충돌 컴포넌트 설정
		playerCollider->SetMobility(Server::EColliderMobility::DYNAMIC);
		playerCollider->SetCollisionLayer(Server::ECollisionLayer::PLAYER);
		playerCollider->SetSize(Shared::vec2(0.3f, 0.6f));
		playerCollider->SetOffset(Shared::vec2(0.0f, -0.2f));
		MGSL_SERVER_COLLISION_MGR.Register(playerCollider);

		Server::GameObject* leftHitbox = MGSL_OBJECT_MGR.CreateGameObject(scenePtr); if (!leftHitbox) return;
		leftHitbox->GetTransform().SetPosition(Shared::vec3(-0.4f, -0.2f, 0.0f));
		Server::BoxCollider* leftHitboxCollider = MGSL_OBJECT_MGR.AddComponent<Server::BoxCollider>(leftHitbox);
		leftHitboxCollider->SetMobility(Server::EColliderMobility::DYNAMIC);
		leftHitboxCollider->SetCollisionLayer(Server::ECollisionLayer::HITBOX);
		leftHitboxCollider->SetTrigger(true);
		leftHitboxCollider->SetSize(Shared::vec2(0.3f, 0.3f));
		player->AddChild(leftHitbox);
		MGSL_SERVER_COLLISION_MGR.Register(leftHitboxCollider);

		Server::GameObject* rightHitbox = MGSL_OBJECT_MGR.CreateGameObject(scenePtr); if (!rightHitbox) return;
		rightHitbox->GetTransform().SetPosition(Shared::vec3(0.4f, -0.2f, 0.0f));
		Server::BoxCollider* rightHitboxCollider = MGSL_OBJECT_MGR.AddComponent<Server::BoxCollider>(rightHitbox);
		rightHitboxCollider->SetMobility(Server::EColliderMobility::DYNAMIC);
		rightHitboxCollider->SetCollisionLayer(Server::ECollisionLayer::HITBOX);
		rightHitboxCollider->SetTrigger(true);
		rightHitboxCollider->SetSize(Shared::vec2(0.3f, 0.3f));
		player->AddChild(rightHitbox);
		MGSL_SERVER_COLLISION_MGR.Register(rightHitboxCollider);

		// 3. GameObject <-> Room / Session 연결
		player->SetGameRoom(GetGameRoom());
		player->SetGameSession(session);
		session->SetGameObject(player);

		// 4. 기존 플레이어들을 신규 플레이어에게 전달
		SendExistingPlayers(session);

		// 5. Room 등록
		const Shared::uint64 objectID = controller->GetObjectID();
		m_players.emplace(objectID, controller);

		// 6. 신규 플레이어에게 자신의 입장 완료 정보 전달, S_EnterGame 전송
		session->GetSessionBuffer().Send(ServerPacketHandler::Create_S_EnterGame(true, controller->GetInfo()));

		// 7. 기존 플레이어들에게 신규 플레이어 전달
		BroadcastSpawn(controller);

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

		Protobuf::S_SpawnPlayer spawnPkt;
		for (const auto& [id, controller] : m_players)
		{
			if (!controller) continue;
			*spawnPkt.add_objects() = controller->GetInfo();
		}

		if (spawnPkt.objects_size() == 0) return;
		MGSL_LOG_INFO("Send S_SpawnPlayer. ObjectCount = {}", spawnPkt.objects_size());
		session->GetSessionBuffer().Send(ServerPacketHandler::Make_S_SpawnPlayer(spawnPkt));
	}

	void GameRoom::BroadcastSpawn(Server::PlayerController* controller)
	{
		if (!controller) return;

		Protobuf::S_SpawnPlayer spawnPkt;
		*spawnPkt.add_objects() = controller->GetInfo();

		const Shared::uint64 newObjectID = controller->GetObjectID();
		for (const auto& [objectID, otherController] : m_players)
		{
			if (!otherController) continue;
			if (objectID == newObjectID) continue;

			Server::GameObject* otherPlayer = otherController->GetOwner();
			if (!otherPlayer) continue;

			GameSessionPtr otherSession = otherPlayer->GetGameSession();
			if (!otherSession) continue;
			otherSession->GetSessionBuffer().Send(ServerPacketHandler::Make_S_SpawnPlayer(spawnPkt));
		}
	}

	void GameRoom::BroadcastSyncObjects()
	{
		::Protobuf::S_SyncPlayers pkt;

		for (const auto& [objectID, controller] : m_players)
		{
			if (!controller) continue;
			*pkt.add_objects() = controller->GetInfo();
		}

		if (pkt.objects_size() == 0) return;

		for (const auto& [objectID, controller] : m_players)
		{
			if (!controller) continue;
			
			Server::GameObject* player = controller->GetOwner();
			if (!player) continue;
		
			GameSessionPtr session = player->GetGameSession();
			if (!session) continue;

			session->GetSessionBuffer().Send(ServerPacketHandler::Make_S_SyncPlayers(pkt));
		}
	}

	GameRoomPtr GameRoom::GetGameRoom()
	{
		return shared_from_this();
	}
}
