#include "GameFrameworkPch.h"
#include "Window.h"

namespace MGSL::Framework
{
    Window::Window() = default;
    Window::~Window()
    {
        if (m_hWnd) ::DestroyWindow(m_hWnd);
        ::UnregisterClass(m_desc.className.c_str(), m_hInstance);
    }

    bool Window::Create(const WindowDesc& desc)
    {
        // 0. 윈도우 정보 구조체 저장
        m_desc = desc;

        // 1. 윈도우 클래스 등록
        WNDCLASSEXW wcx = {};
        wcx.cbSize = sizeof(WNDCLASSEXW);
        wcx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wcx.lpfnWndProc = Window::StaticWndProc;
        wcx.hInstance = m_hInstance;
        wcx.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
        wcx.lpszClassName = m_desc.className.c_str();
        if (!::RegisterClassExW(&wcx)) return false;

        // 윈도우 스타일 및 크기 조정
        DWORD style = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME;
        RECT rect = { 0, 0, m_desc.width, m_desc.height };
        ::AdjustWindowRect(&rect, style, FALSE);

        // 2. 윈도우 생성 (this 포인터를 마지막 인자로 넘김)
        Shared::int32 totalWidth = rect.right - rect.left;
        Shared::int32 totalHeight = rect.bottom - rect.top;
        m_hWnd = ::CreateWindowExW
        (
            0, 
            m_desc.className.c_str(),
            m_desc.title.c_str(),
            style,
            CW_USEDEFAULT, 
            CW_USEDEFAULT, 
            totalWidth,
            totalHeight,
            nullptr, 
            nullptr, 
            m_hInstance, 
            this
        );
        if (!m_hWnd) return false;

        ::ShowWindow(m_hWnd, SW_SHOW);
        ::UpdateWindow(m_hWnd);

        // 생성 직후 전체 화면 여부 적용
        if (m_desc.fullscreen)
        {
            m_desc.fullscreen = false;
            SetFullscreen(true);
        }

        return true;
    }

    void Window::SetFullscreen(bool isFullscreen)
    {
        if (m_desc.fullscreen == isFullscreen)
            return;

        m_desc.fullscreen = isFullscreen;
        DWORD style = ::GetWindowLongW(m_hWnd, GWL_STYLE);

        if (isFullscreen)
        {
            // 현재 모니터 정보 가져오기
            MONITORINFO mi = { sizeof(MONITORINFO) };
            if (::GetWindowPlacement(m_hWnd, &m_windowPosition) &&
                ::GetMonitorInfoW(::MonitorFromWindow(m_hWnd, MONITOR_DEFAULTTOPRIMARY), &mi))
            {
                // 테두리 제거 및 창 위치/크기를 모니터에 맞춤
                ::SetWindowLongW(m_hWnd, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
                ::SetWindowPos(m_hWnd, HWND_TOP,
                    mi.rcMonitor.left, mi.rcMonitor.top,
                    mi.rcMonitor.right - mi.rcMonitor.left,
                    mi.rcMonitor.bottom - mi.rcMonitor.top,
                    SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
            }
        }
        else
        {
            // 테두리 복구 및 이전 창 위치/크기로 복원
            ::SetWindowLongW(m_hWnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
            ::SetWindowPlacement(m_hWnd, &m_windowPosition);
            ::SetWindowPos
            (
                m_hWnd, nullptr, 0, 0, 0, 0,
                SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED
            );
        }
    }

    void Window::ProcessMessages()
    {
        MSG msg = {};
        while (::PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) 
                m_shouldClose = true;

            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
        }
    }

    LRESULT CALLBACK Window::StaticWndProc
    (
        HWND hWnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    )
    {
        if (message == WM_NCCREATE)
        {
            // CreateWindowEx의 마지막 인자로 넘긴 this 포인터를 꺼내옴
            CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
            Window* pWindow = reinterpret_cast<Window*>(pCreate->lpCreateParams);

            // 윈도우 핸들에 실제 객체의 주소(this)를 저장
            ::SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
        }

        // 저장해둔 객체 주소를 꺼내옴
        Window* pWindow = reinterpret_cast<Window*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

        // 객체가 존재하면 해당 인스턴스의 멤버 함수로 메시지 처리를 위임
        if (pWindow)
            return pWindow->MessageHandler(hWnd, message, wParam, lParam);

        return ::DefWindowProcW(hWnd, message, wParam, lParam);
    }

    // 인스턴스별 내부 메시지 처리 로직
    LRESULT Window::MessageHandler
    (
        HWND hWnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam
    )
    {
        switch (message)
        {
        case WM_DESTROY:
            ::PostQuitMessage(0);
            return 0;
        }

        return ::DefWindowProcW(hWnd, message, wParam, lParam);
    }
}