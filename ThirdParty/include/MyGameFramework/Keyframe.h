#pragma once

namespace MGSL::Framework
{
	struct PositionKeyFrame
	{
		float time = 0.0f;
		Shared::vec3 value	{ 0.0f };
	};

	struct RotationKeyFrame
	{
		float time = 0.0f;
		Shared::quat value	{ 1.0f, 0.0f, 0.0f, 0.0f };
	};

	struct ScaleKeyFrame
	{
		float time = 0.0f;
		Shared::vec3 value	{ 1.0f };
	};
}

