#include "GameFrameworkPch.h"
#include "RenderBatchPool.h"
#include "RenderBatch.h"

namespace MGSL::Framework
{
	RenderBatchPool::RenderBatchPool() = default;
	RenderBatchPool::~RenderBatchPool() = default;

	void RenderBatchPool::Clear()
	{
		for (const BatchUPtr& renderBatch : m_renderBatches)
		{
			if (renderBatch)
				renderBatch->Clear();
		}
	}

	const RenderBatchPool::BatchList& RenderBatchPool::GetRenderBatches() const
	{
		return m_renderBatches;
	}

	RenderBatch* RenderBatchPool::StoreBatch(BatchUPtr renderBatch)
	{
		if (!renderBatch) return nullptr;

		RenderBatch* storedBatch = renderBatch.get();
		m_renderBatches.push_back(std::move(renderBatch));
		return storedBatch;
	}
}