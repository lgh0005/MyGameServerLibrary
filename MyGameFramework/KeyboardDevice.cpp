#include "GameFrameworkPch.h"
#include "KeyboardDevice.h"

namespace MGSL::Framework
{
    KeyboardDevice::KeyboardDevice(HWND hwnd) : Super(hwnd) 
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

    KeyboardDevice::~KeyboardDevice() = default;

    void KeyboardDevice::Update(const Shared::uint8* keyStates)
    {
        // 키 상태 가져오기
        if (keyStates == nullptr) return;

        for (Shared::usize key = 0; key < KEY_COUNT; ++key)
        {
            // 최상위 비트(0x80)를 검사하여 키가 눌려 있는지 확인
            if (keyStates[key] & 0x80)
            {
                EInputState& state = m_states[key];

                // 이전 프레임에 눌려있었다면 Press, 아니면 Down
                if (state == EInputState::PRESS || state == EInputState::DOWN)
                    state = EInputState::PRESS;
                else
                    state = EInputState::DOWN;
            }
            else
            {
                EInputState& state = m_states[key];

                // 이전 프레임에 눌려있었다면 떼어진 것이므로 Up, 아니면 None
                if (state == EInputState::PRESS || state == EInputState::DOWN)
                    state = EInputState::UP;
                else
                    state = EInputState::NONE;
            }
        }
    }

    void KeyboardDevice::Clear()
    {
        // 모든 키 상태를 초기화
        ::FillMemory
        (
            m_states.data(),
            sizeof(EInputState) * m_states.size(),
            static_cast<Shared::uint8>(EInputState::NONE)
        );
    }
}