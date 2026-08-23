#pragma once
#include <limits>

namespace MGSL::Framework
{
	/*=========================//
	//   Math Types from glm   //
	//=========================*/
	using len  = glm::length_t;
	using vec2 = glm::vec2;
	using vec3 = glm::vec3;
	using vec4 = glm::vec4;
	using ivec2 = glm::ivec2;
	using ivec3 = glm::ivec3;
	using ivec4 = glm::ivec4;
	using uvec2 = glm::uvec2;
	using uvec3 = glm::uvec3;
	using uvec4 = glm::uvec4;
	using mat2 = glm::mat2;
	using mat3 = glm::mat3;
	using mat4 = glm::mat4;
	using quat = glm::quat;

	/*======================//
	//   Numeric Limits     //
	//======================*/
	template<typename T> using NumericLimits = std::numeric_limits<T>;
	template<typename T> inline constexpr T Epsilon = NumericLimits<T>::epsilon();
	template<typename T> inline constexpr T Max = NumericLimits<T>::max();
	template<typename T> inline constexpr T Lowest = NumericLimits<T>::lowest();
}