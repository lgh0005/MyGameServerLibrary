#pragma once

namespace MGSL::Net
{
	MGSL_CLASS_PTR(GameSession)
	MGSL_CLASS_PTR(SendBuffer)

	class GameSessionManager
	{
		MGSL_DECLARE_SINGLE(GameSessionManager)

	private:
		GameSessionManager();
		~GameSessionManager();

	public:
		void Add(GameSessionPtr session);
		void Remove(GameSessionPtr session);
		void Broadcast(Net::SendBufferPtr sendBuffer);

	private:
		Shared::Mutex m_sessionMutex;
		Shared::HashSet<GameSessionPtr> m_sessions;
	};
}

