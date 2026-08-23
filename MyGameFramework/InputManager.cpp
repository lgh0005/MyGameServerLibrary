#include "GameFrameworkPch.h"
#include "InputManager.h"
#include "KeyboardDevice.h"
#include "MouseDevice.h"

namespace MGSL::Framework
{
	InputManager::InputManager() = default;
	InputManager::~InputManager() = default;

	bool InputManager::Init(HWND hwnd)
	{
		if (hwnd == nullptr)
			return false;

		// 디바이스 생성 및 HWND 전달
		m_keyboardDevice = std::make_unique<KeyboardDevice>(hwnd);
		m_mouseDevice = std::make_unique<MouseDevice>(hwnd);
		if (m_keyboardDevice == nullptr || m_mouseDevice == nullptr)
			return false;

		return true;
	}

	void InputManager::Update()
	{
		if (m_keyboardDevice == nullptr || m_mouseDevice == nullptr)
			return;

		// 1. 프레임당 단 1회, 256개 가상 키 상태를 일괄 캡처
		Shared::uint8 asciiKeys[256] = {};
		if (::GetKeyboardState(asciiKeys) == FALSE)
			return;

		// 2. 캡처된 단일 버퍼를 각 디바이스에 전달
		m_keyboardDevice->Update(asciiKeys);
		m_mouseDevice->Update(asciiKeys);
	}

	void InputManager::Clear()
	{
		if (m_keyboardDevice != nullptr)
			m_keyboardDevice->Clear();

		if (m_mouseDevice != nullptr)
			m_mouseDevice->Clear();
	}
}