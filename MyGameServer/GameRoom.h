#pragma once

namespace MGSL::Server 
{ 
	MGSL_CLASS_PTR(PlayerController)
	MGSL_CLASS_PTR(BulletController)
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
		void Clear();

	public:
		void EnterGameRoom(GameSessionPtr session);
		void LeaveGameRoom(GameSessionPtr session);
		void SpawnBullet(const ::Protobuf::BulletInfo& info);
		void RemoveBullet(Shared::uint64 objectID);
		void BroadcastEffect(const ::Protobuf::EffectInfo& info);

	public:
		Server::PlayerController* FindPlayer(Shared::uint64 objectID) const;

	/*===========================//
	//   Packet Sending Methods  //
	//===========================*/
	private:
		void SendExistingPlayers(GameSessionPtr session);
		void BroadcastPlayerSpawn(Server::PlayerController* controller);
		void BroadcastPlayerRemove(Shared::uint64 objectID);
		void BroadcastSyncPlayers();

		void SendExistingBullets(GameSessionPtr session);
		void BroadcastBulletSpawn(Server::BulletController* controller);
		void BroadcastRemoveBullet(Shared::uint64 objectID);
		void BroadcastSyncBullets();

	private:
		GameRoomPtr GetGameRoom();
		Shared::Dictionary<Shared::uint64, Server::PlayerController*> m_players;
		Shared::Dictionary<Shared::uint64, Server::BulletController*> m_bullets;
		Server::VirtualSandbox2DSceneUPtr m_virtualScene;
	};
}

