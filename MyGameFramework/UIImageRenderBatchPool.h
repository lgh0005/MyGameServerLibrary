#pragma once
#include "RenderBatchPool.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(QuadMesh)
	MGSL_CLASS_PTR(Texture2D)
	MGSL_CLASS_PTR(UIImageRenderBatch)

	class UIImageRenderBatchPool : public RenderBatchPool
	{
	public:
		UIImageRenderBatchPool();
		virtual ~UIImageRenderBatchPool() override;

	public:
		bool Init
		(
			const QuadMeshPtr& quadMesh,
			Shared::uint32 maxInstanceCount
		);

		UIImageRenderBatch* Acquire(const Texture2DPtr& texture);

	private:
		UIImageRenderBatch* GetAvailableBatch(const Texture2DPtr& texture);

	private:
		QuadMeshPtr m_quadMesh;
		Shared::uint32 m_maxInstanceCount = 0;
	};
}

