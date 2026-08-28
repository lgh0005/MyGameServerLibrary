#include "ServerPch.h"
#include "GameSession.h"
#include "ServerPacketHandler.h"
#include "MyGameServerProtocol/PacketHeader.h"
#include "GameRoom.h"
#include "Server.h"

namespace MGSL::Net
{
	GameSession::GameSession() = default;
	GameSession::~GameSession() = default;

	void GameSession::OnConnected()
	{
		MGSL_LOG_INFO("GameSession connected.");
		MGSL_SESSION_MGR.Add(GetGameSession());
	}

	void GameSession::OnDisconnected()
	{
		MGSL_LOG_INFO("GameSession disconnected.");

		GameSessionPtr session = GetGameSession();
		Server::GameObject* gameObject = GetGameObject();
		if (gameObject)
		{
			GameRoomPtr room = gameObject->GetGameRoom();
			if (room) room->LeaveGameRoom(session);
		}

		MGSL_SESSION_MGR.Remove(session);
	}

	void GameSession::OnRecvPacket(BYTE* buffer, Shared::int32 len)
	{
		ServerPacketHandler::HandlePacket(GetGameSession(), buffer, len);
	}

	void GameSession::OnSend(Shared::int32 len)
	{
		// TODO : cout << "OnSend Len = " << len << endl;
	}

	void GameSession::SetGameObject(Server::GameObject* gameObject)
	{
		m_gameObject = gameObject;
	}

	Server::GameObject* GameSession::GetGameObject() const
	{
		return m_gameObject;
	}

	GameSessionPtr GameSession::GetGameSession()
	{
		return static_pointer_cast<GameSession>(shared_from_this());
	}
}
