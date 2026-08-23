#include "SharedPch.h"
#include "ThreadManager.h"

namespace MGSL::Shared
{
	ThreadManager::ThreadManager() = default;
	ThreadManager::~ThreadManager() = default;

	void ThreadManager::Launch(Action<> callback)
	{
		LockGuard lock(m_threadManagerMutex);
		m_threads.push_back(Thread([=]() { callback(); }));
	}

	void ThreadManager::Join()
	{
		for (Thread& t : m_threads)
		{
			if (t.joinable())
				t.join();
		}
		m_threads.clear();
	}
}