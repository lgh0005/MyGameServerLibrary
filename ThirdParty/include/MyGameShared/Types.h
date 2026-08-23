#pragma once

namespace MGSL::Shared
{
	/*===========================//
	//   Primitive Types Alias   //
	//===========================*/
	using int8 = __int8;
	using int16 = __int16;
	using int32 = __int32;
	using int64 = __int64;
	using uint8 = unsigned __int8;
	using uint16 = unsigned __int16;
	using uint32 = unsigned __int32;
	using uint64 = unsigned __int64;
	using usize = size_t;

	/*===========================//
	//   Character Types Alias   //
	//===========================*/
	using char8 = char;
	using char16 = char16_t;
	using char32 = char32_t;
	using wchar = wchar_t;

	using cstr = const char*;
	using wcstr = const wchar_t*;
	using u16str = const char16_t*;
	using u32str = const char32_t*;

	/*=========================//
	//   Math Types from glm   //
	//=========================*/
	using len = glm::length_t;
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
