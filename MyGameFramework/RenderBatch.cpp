#include "GameFrameworkPch.h"
#include "RenderBatch.h"

namespace MGSL::Framework
{
	RenderBatch::RenderBatch(Shared::uint32 maxItemCount)
		: m_maxItemCount(maxItemCount) { }
	RenderBatch::~RenderBatch() = default;

	bool RenderBatch::IsEmpty() const { return m_itemCount == 0; }
	bool RenderBatch::IsFull() const { return m_itemCount >= m_maxItemCount; }
	Shared::uint32 RenderBatch::GetItemCount() const { return m_itemCount; }
	Shared::uint32 RenderBatch::GetMaxItemCount() const { return m_maxItemCount; }
	bool RenderBatch::CanAddItem() const { return m_itemCount < m_maxItemCount; }
	void RenderBatch::IncreaseItemCount() { if (m_itemCount < m_maxItemCount) ++m_itemCount; }
	void RenderBatch::ResetItemCount() { m_itemCount = 0; }
}
