#pragma once
#include "MyGameShared/BlockingQueue.h"

namespace MGSL::Net
{
	MGSL_CLASS_PTR(ServerSession)
	MGSL_CLASS_PTR(ClientService)
	MGSL_CLASS_PTR(SendBuffer)

	class NetworkManager
	{
		MGSL_DECLARE_SINGLE(NetworkManager)
		static constexpr MGSL::Shared::int32 WORKER_COUNT = 5;
		using MainThreadTask = Shared::Action<>;

	private:
		NetworkManager();
		~NetworkManager();

	public:
		bool Init(const Shared::WString& address, Shared::uint16 port);
		void Update();
		void Clear();

	public:
		ServerSessionPtr CreateServerSession();
		void SendPacket(SendBufferPtr buffer);
		bool PushMainThreadTask(MainThreadTask task);

	public:
		template<typename T> bool RunOnMainThread(T&& func);

	private:
		ClientServicePtr m_clientService;
		ServerSessionPtr m_serverSession;
		Shared::Atomic<bool> m_isWorkerRunning = true;
		Shared::BlockingQueue<MainThreadTask> m_mainThreadTasks;
	};
}

#include "NetworkManager.inl"
