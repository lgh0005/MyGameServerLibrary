#pragma once
#include "IInputDevice.h"

namespace MGSL::Framework
{
	class MouseDevice : public IInputDevice
	{
        using Super = IInputDevice;

    public:
        explicit MouseDevice(HWND hwnd);
        virtual ~MouseDevice() override;
        
    public:
        virtual void Update(const Shared::uint8* keyStates) override;
        virtual void Clear() override;

    public:
        void HandleWheel(float delta) { m_wheelDelta = delta; }

        bool GetButtonDown(EMouseButton button) const { return m_states[static_cast<int>(button)] == EInputState::DOWN; }
        bool GetButtonPress(EMouseButton button) const { return m_states[static_cast<int>(button)] == EInputState::PRESS; }
        bool GetButtonUp(EMouseButton button) const { return m_states[static_cast<int>(button)] == EInputState::UP; }

        POINT GetPosition() const { return m_mousePos; }
        Shared::vec2 GetPositionVec() const { return Shared::vec2(m_mousePos.x, m_mousePos.y); }
        POINT GetDelta() const { return m_mouseDelta; }
        Shared::vec2 GetDeltaVec() const { return Shared::vec2(m_mouseDelta.x, m_mouseDelta.y); }
        float GetWheelDelta() const { return m_wheelDelta; }

    private:
        Shared::Array<EInputState, static_cast<Shared::usize>(EMouseButton::COUNT)> m_states{};
        Shared::Array<bool, static_cast<Shared::usize>(EMouseButton::COUNT)>       m_prevStates{};
	
        POINT m_mousePos = { 0, 0 };
        POINT m_mouseDelta = { 0, 0 };

        float m_wheelDelta = 0;
    };
}




