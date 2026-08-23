#pragma once

namespace MGSL::Shared
{
	template<typename T> BlockingQueue<T>::BlockingQueue() = default;
	template<typename T> BlockingQueue<T>::~BlockingQueue() = default;

	template<typename T>
	bool BlockingQueue<T>::Push(T value)
	{
		{
			LockGuard lock(m_queueMutex);
			if (m_closed) return false;
			m_queue.push(std::move(value));
		}

		// 락을 해제한 뒤 대기 중인 스레드 하나를 깨운다.
		m_queueCondVar.notify_one();
		return true;
	}

	template<typename T>
	bool BlockingQueue<T>::TryPop(T& value)
	{
		LockGuard lock(m_queueMutex);

		if (m_queue.empty()) return false;

		value = std::move(m_queue.front());
		m_queue.pop();

		return true;
	}

	template<typename T>
	bool BlockingQueue<T>::WaitPop(T& value)
	{
		UniqueLock lock(m_queueMutex);

		// Spurious Wakeup을 고려하여 조건식을 함께 사용한다.
		m_queueCondVar.wait(lock, [this] { return m_closed || !m_queue.empty(); });
		if (m_queue.empty()) return false;

		value = std::move(m_queue.front());
		m_queue.pop();

		return true;
	}

	template<typename T>
	void BlockingQueue<T>::Close()
	{
		{
			LockGuard lock(m_queueMutex);
			if (m_closed) return;
			m_closed = true;
		}

		m_queueCondVar.notify_all();
	}
}