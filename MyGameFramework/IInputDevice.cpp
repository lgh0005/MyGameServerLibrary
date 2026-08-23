#include "GameFrameworkPch.h"
#include "IInputDevice.h"

namespace MGSL::Framework
{
	IInputDevice::IInputDevice(HWND hwnd) : m_currentWindowHandle(hwnd) { }
	IInputDevice::~IInputDevice() = default;
}