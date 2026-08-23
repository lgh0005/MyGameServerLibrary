#include "GameFrameworkPch.h"
#include "InstancedRenderBatch.h"
#include "GLVertexArray.h"
#include "GLBuffer.h"

namespace MGSL::Framework
{
	InstancedRenderBatch::InstancedRenderBatch(Shared::uint32 maxInstanceCount) : Super(maxInstanceCount) {}
	InstancedRenderBatch::~InstancedRenderBatch() = default;
}