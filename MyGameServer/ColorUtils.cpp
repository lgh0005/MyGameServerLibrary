#include "ServerPch.h"
#include "ColorUtils.h"

namespace MGSL::Server
{
	Shared::vec3 ColorUtils::HSVToRGB
	(
		float hue,
		float saturation,
		float value
	)
	{
		const float c = value * saturation;
		const float x = c * (1.0f - std::abs(std::fmod(hue / 60.0f, 2.0f) - 1.0f));
		const float m = value - c;

		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;

		if (hue < 60.0f) { r = c; g = x; }
		else if (hue < 120.0f) { r = x; g = c; }
		else if (hue < 180.0f) { g = c; b = x; }
		else if (hue < 240.0f) { g = x; b = c; }
		else if (hue < 300.0f) { r = x; b = c; }
		else { r = c; b = x; }

		return Shared::vec3(r + m, g + m, b + m);
	}
}
