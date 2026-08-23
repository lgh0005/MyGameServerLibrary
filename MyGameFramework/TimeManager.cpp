#include "GameFrameworkPch.h"
#include "TimeManager.h"

namespace MGSL::Framework
{
	TimeManager::TimeManager() = default;
	TimeManager::~TimeManager() = default;

    void TimeManager::Init()
    {
        // CPU의 타이머 주파수(1초당 틱 수)를 획득.
        ::QueryPerformanceFrequency(&m_cpuFrequency);

        // 초기 시간을 기록.
        ::QueryPerformanceCounter(&m_prevTime);
    }

    void TimeManager::Update()
    {
        // 현재 시간을 갱신.
        ::QueryPerformanceCounter(&m_currentTime);

        // 프레임 간 경과 시간(Delta Time)을 초 단위로 계산.
        m_deltaTime = static_cast<float>(m_currentTime.QuadPart - m_prevTime.QuadPart) /
            static_cast<float>(m_cpuFrequency.QuadPart);

        // 다음 프레임 계산을 위해 현재 시간을 저장합.
        m_prevTime = m_currentTime;

        // FPS 측정 로직
        m_fpsCount++;
        m_fpsTimer += m_deltaTime;

        // 1초가 경과했을 때 FPS를 갱신.
        if (m_fpsTimer >= 1.0f)
        {
            m_fps = m_fpsCount;
            m_fpsCount = 0;

            // 0.0f로 초기화하는 대신 1.0f를 빼주어 누적 오차를 방지합.
            m_fpsTimer -= 1.0f;
        }
    }

    void TimeManager::Clear()
    {
        // TODO
    }
}