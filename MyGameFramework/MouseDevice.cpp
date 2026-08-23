#include "GameFrameworkPch.h"
#include "MouseDevice.h"

namespace MGSL::Framework
{
    MouseDevice::MouseDevice(HWND hwnd) : Super(hwnd)
    {
        ::FillMemory
        (
            m_states.data(),
            sizeof(EInputState) * m_states.size(),
            static_cast<Shared::uint8>(EInputState::NONE)
        );

        ::FillMemory
        (
            m_prevStates.data(),
            sizeof(EInputState) * m_prevStates.size(),
            false
        );
    }

    MouseDevice::~MouseDevice() = default;

    void MouseDevice::Update(const Shared::uint8* keyStates)
    {
        if (m_currentWindowHandle == nullptr || keyStates == nullptr)
            return;

        // 1. 마우스 좌표 및 변위(Delta) 업데이트
        POINT currentPos = { 0, 0 };
        ::GetCursorPos(&currentPos);
        ::ScreenToClient(m_currentWindowHandle, &currentPos);

        m_mouseDelta.x = currentPos.x - m_mousePos.x;
        m_mouseDelta.y = currentPos.y - m_mousePos.y;
        m_mousePos = currentPos;

        // 2. 마우스 버튼 상태 업데이트
        const Shared::int32 vkButtons[] = { VK_LBUTTON, VK_RBUTTON, VK_MBUTTON };

        for (Shared::usize i = 0; i < static_cast<Shared::int32>(EMouseButton::COUNT); ++i)
        {
            // 최상위 비트(0x80)를 검사하여 키가 눌려 있는지 확인
            if (keyStates[vkButtons[i]] & 0x80)
            {
                EInputState& state = m_states[i];

                // 이전 프레임에 눌려있었다면 Press, 아니면 Down
                if (state == EInputState::PRESS || state == EInputState::DOWN)
                    state = EInputState::PRESS;
                else
                    state = EInputState::DOWN;
            }
            else
            {
                EInputState& state = m_states[i];

                // 이전 프레임에 눌려있었다면 떼어진 것이므로 Up, 아니면 None
                if (state == EInputState::PRESS || state == EInputState::DOWN)
                    state = EInputState::UP;
                else
                    state = EInputState::NONE;
            }
        }
    }

    void MouseDevice::Clear()
    {
        ::FillMemory
        (
            m_states.data(), 
            sizeof(EInputState) * m_states.size(), 
            static_cast<Shared::uint8>(EInputState::NONE)
        );

        m_wheelDelta = 0.0f;
        m_mouseDelta = { 0, 0 };
    }
}