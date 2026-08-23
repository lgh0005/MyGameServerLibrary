#pragma once

namespace MGSL::Server
{
	class ServerTimeManager
	{
		MGSL_DECLARE_SINGLE(ServerTimeManager)

	private:
		ServerTimeManager();
		~ServerTimeManager();

		// Server Tick
		static constexpr float TICK_RATE = 30.0f;
		static constexpr float TICK_INTERVAL = 1.0f / TICK_RATE;

	public:
		void Init();
		bool Update();
		void Clear();

	public:
		float GetDeltaTime() const { return m_deltaTime; }
		float GetFixedDeltaTime() const { return TICK_INTERVAL; }
		Shared::uint32 GetFPS() const { return m_fps; }

	private:
		LARGE_INTEGER m_cpuFrequency{};
		LARGE_INTEGER m_prevTime{};
		LARGE_INTEGER m_currentTime{};

		float m_deltaTime = 0.0f;
		float m_tickAccumulator = 0.0f;

		float m_fpsTimer = 0.0f;
		Shared::uint32 m_fpsCount = 0;
		Shared::uint32 m_fps = 0;
	};
}

