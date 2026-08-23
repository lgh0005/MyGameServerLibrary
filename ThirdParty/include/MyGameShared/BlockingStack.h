#pragma once

namespace MGSL::Shared
{
	template<typename T>
	class BlockingStack
	{
		MGSL_DISABLE_COPY(BlockingStack)
		MGSL_DISABLE_MOVE(BlockingStack)

	public:
		BlockingStack();
		~BlockingStack();

	public:
		bool Push(T value);
		bool TryPop(T& value);
		bool WaitPop(T& value);
		void Close();

	private:
		Stack<T> m_stack;
		Mutex m_stackMutex;
		CondVar m_stackCondVar;
		bool m_closed = false;
	};
}

#include "BlockingStack.inl"