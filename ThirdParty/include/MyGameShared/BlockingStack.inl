#pragma once

namespace MGSL::Shared
{
	template<typename T> BlockingStack<T>::BlockingStack() = default;
	template<typename T> BlockingStack<T>::~BlockingStack() = default;

	template<typename T>
	bool BlockingStack<T>::Push(T value)
	{
		{
			LockGuard lock(m_stackMutex);
			if (m_closed) return false;
			m_stack.push(std::move(value));
		}

		// 스택에 데이터가 추가되었으므로 대기 중인 스레드 하나를 깨운다.
		m_stackCondVar.notify_one();
		return true;
	}

	template<typename T>
	bool BlockingStack<T>::TryPop(T& value)
	{
		LockGuard lock(m_stackMutex);

		if (m_stack.empty()) return false;

		value = std::move(m_stack.top());
		m_stack.pop();

		return true;
	}

	template<typename T>
	bool BlockingStack<T>::WaitPop(T& value)
	{
		UniqueLock lock(m_stackMutex);

		// 허위 기상까지 고려하여 스택이 비어 있지 않을 때까지 대기한다.
		m_stackCondVar.wait(lock, [this] { return m_closed || !m_stack.empty(); });
		if (m_stack.empty()) return false;

		value = std::move(m_stack.top());
		m_stack.pop();

		return true;
	}

	template<typename T>
	void BlockingStack<T>::Close()
	{
		{
			LockGuard lock(m_stackMutex);
			if (m_closed) return;
			m_closed = true;
		}

		// WaitPop 중인 모든 스레드를 깨운다.
		m_stackCondVar.notify_all();
	}
}