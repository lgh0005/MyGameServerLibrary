#pragma once
#include "RenderBatch.h"

namespace MGSL::Framework   
{
	MGSL_CLASS_PTR(GLVertexArray)
	MGSL_CLASS_PTR(GLBuffer)

	class InstancedRenderBatch : public RenderBatch
	{
		MGSL_DISABLE_COPY(InstancedRenderBatch)
		MGSL_DISABLE_MOVE(InstancedRenderBatch)
		using Super = RenderBatch;

	public:
		virtual ~InstancedRenderBatch();
		virtual bool SetInstanceVertexArrayAttrib() = 0;

	protected:
		InstancedRenderBatch(Shared::uint32 maxInstanceCount);

		GLVertexArrayUPtr m_instanceVertexArray;
		GLBufferUPtr m_instanceBuffer;
	};
}
