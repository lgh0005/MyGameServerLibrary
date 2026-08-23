#include "ServerPch.h"
#include "GameSessionManager.h"
#include "GameSession.h"
#include "MyGameServerLibrary/SendBuffer.h"

namespace MGSL::Net
{
	GameSessionManager::GameSessionManager() = default;
	GameSessionManager::~GameSessionManager() = default;

	void GameSessionManager::Add(GameSessionPtr session)
	{
		Shared::LockGuard lock(m_sessionMutex);
		m_sessions.insert(session);
	}

	void GameSessionManager::Remove(GameSessionPtr session)
	{
		Shared::LockGuard lock(m_sessionMutex);
		m_sessions.erase(session);
	}

	void GameSessionManager::Broadcast(Net::SendBufferPtr sendBuffer)
	{
		Shared::LockGuard lock(m_sessionMutex);
		for (GameSessionPtr session : m_sessions)
			session->GetSessionBuffer().Send(sendBuffer);
	}
}