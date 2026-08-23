#pragma once

namespace MGSL::Shared
{
	template<typename T>
	class BlockingQueue
	{
		MGSL_DISABLE_COPY(BlockingQueue)
		MGSL_DISABLE_MOVE(BlockingQueue)

	public:
		BlockingQueue();
		~BlockingQueue();

	public:
		bool Push(T value);
		bool TryPop(T& value);
		bool WaitPop(T& value);
		void Close();

	private:
		Queue<T> m_queue;
		Mutex m_queueMutex;
		CondVar m_queueCondVar;
		bool m_closed = false;
	};
}

#include "BlockingQueue.inl"