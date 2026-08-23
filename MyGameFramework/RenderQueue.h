#pragma once

namespace MGSL::Framework
{
	template<typename T>
	class RenderQueue
	{
	public:
		RenderQueue();
		~RenderQueue();

	public:
		template<typename U> void Sort(U compare);
		void Submit(const T& renderItem);
		void Clear();

	public:
		const Shared::List<T>& GetRenderItems() const;
		Shared::usize GetItemCount() const;
		bool IsEmpty() const;

	private:
		Shared::List<T> m_renderItems;
	};
}

#include "RenderQueue.inl"