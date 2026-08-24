#pragma once
#include "MyGameServerLibrary/PacketSession.h"

namespace MGSL::Server { MGSL_CLASS_PTR(GameObject) }

namespace MGSL::Net
{
	MGSL_CLASS_PTR(GameSession)

	class GameSession : public Net::PacketSession
	{
	public:
		GameSession();
		virtual ~GameSession() override;
		
	public:
		virtual void OnConnected() override;
		virtual void OnDisconnected() override;
		virtual void OnRecvPacket(BYTE* buffer, Shared::int32 len) override;
		virtual void OnSend(Shared::int32 len) override;

	public:
		void SetGameObject(Server::GameObject* gameObject);
		Server::GameObject* GetGameObject() const;

	private:
		GameSessionPtr GetGameSession();

		Server::GameObject* m_gameObject = nullptr;
	};
}

