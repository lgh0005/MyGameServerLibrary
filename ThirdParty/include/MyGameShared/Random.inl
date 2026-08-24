#pragma once

namespace MGSL::Shared
{
	template<typename T>
	T Random::Range(T min, T max)
	{
		if constexpr (std::is_integral_v<T>)
		{
			std::uniform_int_distribution<T> dist(min, max);
			return dist(GetEngine());
		}
		else if constexpr (std::is_floating_point_v<T>)
		{
			std::uniform_real_distribution<T> dist(min, max);
			return dist(GetEngine());
		}
		else
		{
			MGSL_STATIC_ASSERT_MSG
			(
				std::is_integral_v<T> || std::is_floating_point_v<T>,
				"Random::Range supports only integral or floating-point types."
			);
		}
	}

	inline float Random::Value() { return Range(0.0f, 1.0f); }
}