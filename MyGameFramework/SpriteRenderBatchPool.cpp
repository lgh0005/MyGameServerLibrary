#include "GameFrameworkPch.h"
#include "SpriteRenderBatchPool.h"
#include "SpriteRenderBatch.h"

namespace MGSL::Framework
{
	SpriteRenderBatchPool::SpriteRenderBatchPool() = default;
	SpriteRenderBatchPool::~SpriteRenderBatchPool() = default;

	bool SpriteRenderBatchPool::Init
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

	SpriteRenderBatch* SpriteRenderBatchPool::Acquire(const Texture2DPtr& atlasTexture)
	{
		if (!m_quadMesh || !atlasTexture)
			return nullptr;

		SpriteRenderBatch* available = GetAvailableBatch(atlasTexture);
		if (available) return available;

		auto renderBatch = SpriteRenderBatch::Create(m_quadMesh, atlasTexture, m_maxInstanceCount);
		if (!renderBatch) return nullptr;

		SpriteRenderBatch* storedBatch = renderBatch.get();
		StoreBatch(std::move(renderBatch));

		return storedBatch;
	}

	SpriteRenderBatch* SpriteRenderBatchPool::GetAvailableBatch(const Texture2DPtr& atlasTexture)
	{
		for (const BatchUPtr& renderBatch : GetRenderBatches())
		{
			if (!renderBatch || renderBatch->IsFull())
				continue;

			SpriteRenderBatch* spriteRenderBatch = static_cast<SpriteRenderBatch*>(renderBatch.get());
			if (spriteRenderBatch->GetAtlasTexture() != atlasTexture)
				continue;

			return spriteRenderBatch;
		}

		return nullptr;
	}
}