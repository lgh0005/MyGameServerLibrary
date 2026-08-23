#pragma once

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(KeyboardDevice)
	MGSL_CLASS_PTR(MouseDevice)

	class InputManager
	{
		MGSL_DECLARE_SINGLE(InputManager)
	
	private:
		InputManager();
		~InputManager();

	public:
		bool Init(HWND hwnd);
		void Update();
		void Clear();

	public:
		KeyboardDevice* GetKeyboard() const { return m_keyboardDevice.get(); }
		MouseDevice* GetMouse() const { return m_mouseDevice.get(); }

	private:
		KeyboardDeviceUPtr m_keyboardDevice = nullptr;
		MouseDeviceUPtr m_mouseDevice = nullptr;
	};
}

