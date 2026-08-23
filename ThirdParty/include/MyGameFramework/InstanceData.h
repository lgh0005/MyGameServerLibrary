#pragma once

namespace MGSL::Framework
{
	struct alignas(OPENGL_MEMORY_ALIGNMNET) SpriteInstanceData
	{
		Shared::mat4 worldMatrix{ 1.0f };

		// x: uOffset, y: vOffset, z: uWidth, w: vHeight
		Shared::vec4 uvRect{ 0.0f, 0.0f, 1.0f, 1.0f };

		Shared::vec4 color{ 1.0f };
		Shared::vec2 size{ 1.0f };
		Shared::vec2 pad0{ 0.0f };
	};

	struct alignas(OPENGL_MEMORY_ALIGNMNET) UIInstanceData
	{
		Shared::mat4 worldMatrix{ 1.0f };

		// x: uOffset, y: vOffset, z: uWidth, w: vHeight
		Shared::vec4 uvRect{ 0.0f, 0.0f, 1.0f, 1.0f };

		Shared::vec4 color{ 1.0f };
		Shared::vec2 size{ 1.0f };
		Shared::vec2 pad0{ 0.0f };
	};
}

