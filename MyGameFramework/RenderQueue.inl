#pragma once

namespace MGSL::Framework
{
	template<typename T> RenderQueue<T>::RenderQueue() = default;
	template<typename T> RenderQueue<T>::~RenderQueue() = default;

	template<typename T>
	template<typename U>
	void RenderQueue<T>::Sort(U compare)
	{
		std::stable_sort(m_renderItems.begin(), m_renderItems.end(), compare);
	}

	template<typename T>
	void RenderQueue<T>::Submit(const T& renderItem)
	{
		m_renderItems.push_back(renderItem);
	}

	template<typename T>
	void RenderQueue<T>::Clear()
	{
		m_renderItems.clear();
	}

	template<typename T>
	const Shared::List<T>& RenderQueue<T>::GetRenderItems() const
	{
		return m_renderItems;
	}

	template<typename T>
	Shared::usize RenderQueue<T>::GetItemCount() const
	{
		return m_renderItems.size();
	}

	template<typename T>
	bool RenderQueue<T>::IsEmpty() const
	{
		return m_renderItems.empty();
	}
}