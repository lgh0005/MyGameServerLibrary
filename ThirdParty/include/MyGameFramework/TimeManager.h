#pragma once

namespace MGSL::Framework
{
	class TimeManager
	{
		MGSL_DECLARE_SINGLE(TimeManager)

	private:
		TimeManager();
		~TimeManager();

	public:
		void Init();
		void Update();
		void Clear();

	public:
		float GetDeltaTime() const { return m_deltaTime; }
		Shared::uint32 GetFPS() const { return m_fps; }

	private:
		LARGE_INTEGER m_cpuFrequency {};
		LARGE_INTEGER m_prevTime {};
		LARGE_INTEGER m_currentTime {};

		float m_deltaTime = 0.0f;

		float m_fpsTimer = 0.0f;
		Shared::uint32 m_fpsCount = 0;
		Shared::uint32 m_fps = 0;
	};
}

