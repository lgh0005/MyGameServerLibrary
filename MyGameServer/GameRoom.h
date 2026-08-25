#pragma once

namespace MGSL::Server 
{ 
	MGSL_CLASS_PTR(PlayerController)
	MGSL_CLASS_PTR(VirtualSandbox2DScene)
}

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

	/*===========================//
	//   Packet Sending Methods  //
	//===========================*/
	private:
		void SendExistingPlayers(GameSessionPtr session);
		void BroadcastSpawn(Server::PlayerController* controller);
		void BroadcastSyncObjects();

	private:
		GameRoomPtr GetGameRoom();

		Shared::Dictionary<Shared::uint64, Server::PlayerController*> m_players;
		Server::VirtualSandbox2DSceneUPtr m_virtualScene;
	};
}

