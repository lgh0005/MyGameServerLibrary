#pragma once

namespace MGSL::Framework
{
	struct alignas(OPENGL_MEMORY_ALIGNMNET) CameraData
	{
		Shared::mat4 view					  { 1.0 };
		Shared::mat4 projection				  { 1.0 };
		Shared::mat4 viewProjection				  { 1.0 };

		// xyz : Camera World Position
		// w   : Padding
		Shared::vec4 worldPosition				  { 0.0f, 0.0f, 0.0f, 1.0f };
	};
}
