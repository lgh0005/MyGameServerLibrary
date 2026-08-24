#pragma once
#include <random>

namespace MGSL::Shared
{
	class Random
	{
		MGSL_DISABLE_COPY(Random)
		MGSL_DISABLE_MOVE(Random)

	public:
		Random() = delete;
		~Random() = delete;

	public:
		template<typename T> static T Range(T min, T max);
		static float Value();

	private:
		static std::mt19937& GetEngine()
		{
			thread_local std::mt19937 engine { std::random_device{}() };
			return engine;
		}
	};
}

#include "Random.inl"

