#pragma once
#include "RenderBatchPool.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(QuadMesh)
	MGSL_CLASS_PTR(SpriteRenderBatch)

	class SpriteRenderBatchPool : public RenderBatchPool
	{
	public:
		SpriteRenderBatchPool();
		virtual ~SpriteRenderBatchPool() override;

	public:
		bool Init
		(
			const QuadMeshPtr& quadMesh,
			Shared::uint32 maxInstanceCount
		);
		SpriteRenderBatch* Acquire(const Texture2DPtr& atlasTexture);

	private:
		SpriteRenderBatch* GetAvailableBatch(const Texture2DPtr& atlasTexture);

	private:
		QuadMeshPtr m_quadMesh;
		Shared::uint32 m_maxInstanceCount = 0;
	};
}
