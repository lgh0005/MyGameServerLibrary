#pragma once

namespace MGSL::Server { MGSL_CLASS_PTR(GameObject) }

namespace MGSL::Net
{
	MGSL_CLASS_PTR(GameSession)
	MGSL_CLASS_PTR(GameRoom)

	class GameRoom : public Shared::Self<GameRoom>
	{
		MGSL_DISABLE_COPY(GameRoom)
		MGSL_DISABLE_MOVE(GameRoom)

	public:
		GameRoom();
		virtual ~GameRoom();

	public:
		void Init();
		void Update(float deltaTime);

	public:
		void EnterGameRoom(GameSessionPtr session);
		void LeaveGameRoom(GameSessionPtr session);

	/*========================//
	//   Packet Test Methods  //
	//========================*/
	private:
		void SendExistingPlayers(GameSessionPtr session);
		void BroadcastSpawn(Server::GameObjectPtr player);
		void BroadcastSyncObjects();

	private:
		GameRoomPtr GetGameRoom();

		Shared::Dictionary<Shared::uint64, Server::GameObjectPtr> m_players;

		// TEMP : 맵 정보에 대한 충돌체들
		Shared::List<Server::GameObjectPtr> m_staticObjects;  // 맵의 정적 충돌체들
		Shared::List<Server::GameObjectPtr> m_dynamicObjects; // 플레이어 히트박스들
	};
}

