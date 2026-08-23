#include "ServerPch.h"
#include "ServerTimeManager.h"

namespace MGSL::Server
{
    ServerTimeManager::ServerTimeManager() = default;
    ServerTimeManager::~ServerTimeManager() = default;

    void ServerTimeManager::Init()
    {
        // CPU의 타이머 주파수(1초당 틱 수)를 획득.
        ::QueryPerformanceFrequency(&m_cpuFrequency);

        // 초기 시간을 기록.
        ::QueryPerformanceCounter(&m_prevTime);
    }

    bool ServerTimeManager::Update()
    {
        // 현재 시간
        ::QueryPerformanceCounter(&m_currentTime);

        // 프레임 간 경과 시간(Delta Time)을 초 단위로 계산
        m_deltaTime = static_cast<float>(m_currentTime.QuadPart - m_prevTime.QuadPart) /
            static_cast<float>(m_cpuFrequency.QuadPart);

        // 다음 프레임 계산을 위해 현재 시간을 저장
        m_prevTime = m_currentTime;

        // Tick 시간 누적
        m_tickAccumulator += m_deltaTime;

        // 아직 다음 Tick 시간이 되지 않음
        if (m_tickAccumulator < TICK_INTERVAL)
            return false;

        // 한 Tick만큼 소비
        m_tickAccumulator -= TICK_INTERVAL;
        return true;
    }

    void ServerTimeManager::Clear()
    {
        // TODO
    }
}