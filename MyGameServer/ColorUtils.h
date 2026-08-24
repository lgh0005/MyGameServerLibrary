#pragma once

namespace MGSL::Server
{
	class ColorUtils
	{
		MGSL_DISABLE_COPY(ColorUtils)
		MGSL_DISABLE_MOVE(ColorUtils)

	public:
		ColorUtils() = delete;
		~ColorUtils() = delete;

	public:
		static Shared::vec3 HSVToRGB
		(
			float hue,
			float saturation,
			float value
		);
	};
}

