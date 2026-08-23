#pragma once
#include "MyGameShared/Types.h"

namespace MGSL::Framework
{
	static constexpr Shared::int32 OPENGL_MEMORY_ALIGNMNET = 16;

	struct alignas(OPENGL_MEMORY_ALIGNMNET) QuadMeshVertex
	{
		Shared::vec3		  position;
		Shared::vec2		  texCoord;
		Shared::vec3		  pad0;
	};
}