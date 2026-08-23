#pragma once
#include "RenderBatchPool.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(QuadMesh)
	MGSL_CLASS_PTR(Font)
	MGSL_CLASS_PTR(UITextRenderBatch)

	class UITextRenderBatchPool : public RenderBatchPool
	{
	public:
		UITextRenderBatchPool();
		virtual ~UITextRenderBatchPool() override;

	public:
		bool Init
		(
			const QuadMeshPtr& quadMesh,
			Shared::uint32 maxInstanceCount
		);

		UITextRenderBatch* Acquire(const FontPtr& font);

	private:
		UITextRenderBatch* GetAvailableBatch(const FontPtr& font);

	private:
		QuadMeshPtr m_quadMesh;
		Shared::uint32 m_maxInstanceCount = 0;
	};
}
