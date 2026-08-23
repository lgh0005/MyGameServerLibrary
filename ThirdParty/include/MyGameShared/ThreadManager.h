#pragma once

namespace MGSL::Shared
{
	class ThreadManager
	{
		MGSL_DECLARE_SINGLE(ThreadManager)

	private:
		ThreadManager();
		~ThreadManager();

	public:
		void Launch(Action<> threadMain);
		void Join();

	private:
		Mutex m_threadManagerMutex;
		List<Thread> m_threads;
	};
}

