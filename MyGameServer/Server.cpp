#include "ServerPch.h"
#include "Server.h"
#include "GameSession.h"
#include "GameRoom.h"
#include "MyGameServerLibrary/ServerService.h"
#include "MyGameServerLibrary/NetIOCPCore.h"

namespace MGSL::Server
{
	Server::Server() = default;
	Server::~Server() = default;

	bool Server::Init(const Shared::WString& ipAddr, Shared::uint16 port)
	{
		// Logger
		MGSL::Shared::Logger::Instance();
		MGSL_LOG_FATAL("This is Server");

		// Socket Init
		MGSL::Net::SocketUtils::Init();

		// Create Server Serivce
		m_serverService = std::make_shared<Net::ServerService>
		(
			Net::NetAddress(ipAddr, port),
			std::make_shared<Net::NetIOCPCore>(),
			[]() { return std::make_shared<Net::GameSession>(); },
			MAX_SESSION_COUNT
		);

		// ServerService Start
		if (!m_serverService->Start())
		{
			MGSL_LOG_ERROR("Failed to start server service.");
			return false;
		}

		// Create IOCP worker threads
		for (MGSL::Shared::int32 i = 0; i < IOCP_WORKER_THREAD_COUNT; i++)
			MGSL_THREAD_MGR.Launch([=]() { while (true) { m_serverService->GetIOCPCore()->Dispatch(); } });

		// Manager Initialization
		MGSL_SERVER_TIME_MGR.Init();
		MGSL_SERVER_COLLISION_MGR.Init();

		// Create GameRoom Instance
		m_gameRoom = std::make_shared<Net::GameRoom>();
		if (!m_gameRoom) return false;
		m_gameRoom->Init();

		return true;
	}

	void Server::Update()
	{
		while (true)
		{
			if (!MGSL_SERVER_TIME_MGR.Update()) continue;
			m_gameRoom->Update(MGSL_SERVER_TIME_MGR.GetFixedDeltaTime());
		}
	}

	void Server::Clear()
	{
		m_gameRoom->Clear();
		MGSL_SERVER_COLLISION_MGR.Clear();
		MGSL_THREAD_MGR.Join();
		Net::SocketUtils::Clear();
	}

	Net::GameRoomPtr Server::GetGameRoom() const
	{
		return m_gameRoom;
	}
}