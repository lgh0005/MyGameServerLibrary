#pragma once

namespace MGSL::Framework
{
	class IInputDevice
	{
    public:
        IInputDevice(HWND hwnd);
        virtual ~IInputDevice();

    public:
        virtual void Update(const Shared::uint8* keyStates) = 0;
        virtual void Clear() = 0;

    protected:
        HWND m_currentWindowHandle = nullptr;
	};
}

