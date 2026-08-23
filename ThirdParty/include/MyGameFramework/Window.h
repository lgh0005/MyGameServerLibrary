#pragma once

namespace MGSL::Framework
{
	struct WindowDesc
	{
		Shared::WString className{ L"MGSL_WindowClass" };
		Shared::WString title{ L"MySandbox" };
		Shared::int32 width = 1280;
		Shared::int32 height = 720;
		bool fullscreen = false;
	};

	class Window
	{
	public:
		Window();
		virtual ~Window();

	public:
		bool Create(const WindowDesc& desc);
		void ProcessMessages();
		bool ShouldClose() const { return m_shouldClose; }
		void SetFullscreen(bool isFullscreen);

		HWND GetHWND() const { return m_hWnd; }
		Shared::WString GetWindowTitle() const { return m_desc.title; }
		Shared::int32 GetWindowWidth() const { return m_desc.width; }
		Shared::int32 GetWindowHeight() const { return m_desc.height; }

	protected:
		virtual LRESULT MessageHandler
		(
			HWND hWnd,
			UINT message,
			WPARAM wParam,
			LPARAM lParam
		);

	private:
		static LRESULT CALLBACK StaticWndProc
		(
			HWND hWnd,
			UINT message,
			WPARAM wParam,
			LPARAM lParam
		);

	private:
		HWND			m_hWnd = nullptr;
		HINSTANCE		m_hInstance = nullptr;
		WINDOWPLACEMENT m_windowPosition = { sizeof(WINDOWPLACEMENT) };
		WindowDesc		m_desc;
		bool			m_shouldClose = false;
	};
}

