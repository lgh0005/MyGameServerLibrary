#include "GameFrameworkPch.h"
#include "UIImageRenderBatchPool.h"
#include "UIImageRenderBatch.h"

namespace MGSL::Framework
{
	UIImageRenderBatchPool::UIImageRenderBatchPool() = default;
	UIImageRenderBatchPool::~UIImageRenderBatchPool() = default;

	bool UIImageRenderBatchPool::Init
	(
		const QuadMeshPtr& quadMesh,
		Shared::uint32 maxInstanceCount
	)
	{
		if (!quadMesh || maxInstanceCount == 0) return false;
		m_quadMesh = quadMesh;
		m_maxInstanceCount = maxInstanceCount;

		return true;
	}

	UIImageRenderBatch* UIImageRenderBatchPool::Acquire(const Texture2DPtr& texture)
	{
		if (!m_quadMesh || !texture)
			return nullptr;

		UIImageRenderBatch* available = GetAvailableBatch(texture);
		if (available) return available;

		auto renderBatch = UIImageRenderBatch::Create(m_quadMesh, texture, m_maxInstanceCount);
		if (!renderBatch) return nullptr;

		UIImageRenderBatch* storedBatch = renderBatch.get();
		StoreBatch(std::move(renderBatch));

		return storedBatch;
	}

	UIImageRenderBatch* UIImageRenderBatchPool::GetAvailableBatch(const Texture2DPtr& texture)
	{
		for (const BatchUPtr& renderBatch : GetRenderBatches())
		{
			if (!renderBatch || renderBatch->IsFull()) continue;

			UIImageRenderBatch* imageRenderBatch = static_cast<UIImageRenderBatch*>(renderBatch.get());
			if (imageRenderBatch->GetTexture() != texture) continue;

			return imageRenderBatch;
		}

		return nullptr;
	}
}