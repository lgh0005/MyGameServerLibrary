#include "2DPch.h"
#include "NetworkManager.h"
#include "MyGameServerLibrary/NetIOCPCore.h"
#include "MyGameServerLibrary/ClientService.h"
#include "ServerSession.h"

namespace MGSL::Net
{
	NetworkManager::NetworkManager() = default;
	NetworkManager::~NetworkManager() = default;

	bool NetworkManager::Init(const Shared::WString& address, Shared::uint16 port)
	{
		SocketUtils::Init();

		m_clientService = std::make_shared<ClientService>
		(
			NetAddress(address, port),
			std::make_shared<NetIOCPCore>(),
			[=]() { return CreateServerSession(); },
			1
		);
		if (!m_clientService->Start()) return false;

		m_isWorkerRunning.store(true);
		for (MGSL::Shared::int32 i = 0; i < WORKER_COUNT; i++)
			MGSL_THREAD_MGR.Launch([=]() { while (m_isWorkerRunning) { m_clientService->GetIOCPCore()->Dispatch(); } });

		return true;
	}

	void NetworkManager::Update()
	{
		MainThreadTask task;
		while (m_mainThreadTasks.TryPop(task))
		{
			if (task) task();
			task = nullptr;
		}
	}

	void NetworkManager::Clear()
	{
		m_isWorkerRunning.store(false);
		if (m_serverSession && m_serverSession->IsConnected())
			m_serverSession->Disconnect("NetworkManager::Stop");

		for (int i = 0; i < WORKER_COUNT; ++i)
		{
			::PostQueuedCompletionStatus
			(
				m_clientService->GetIOCPCore()->GetNetIOCPHandle(),
				0,
				0,
				nullptr
			);
		}

		SocketUtils::Clear();
	}

	ServerSessionPtr NetworkManager::CreateServerSession()
	{
		m_serverSession = std::make_shared<ServerSession>();
		return m_serverSession;
	}

	void NetworkManager::SendPacket(SendBufferPtr buffer)
	{
		if (m_serverSession) m_serverSession->GetSessionBuffer().Send(buffer);
	}

	bool NetworkManager::PushMainThreadTask(MainThreadTask task)
	{
		if (!task) return false;
		return m_mainThreadTasks.Push(std::move(task));
	}
}