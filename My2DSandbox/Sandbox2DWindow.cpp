#include "2DPch.h"
#include "Sandbox2DWindow.h"

namespace MGSL::Sandbox2D
{
	Sandbox2DWindow::Sandbox2DWindow() = default;
	Sandbox2DWindow::~Sandbox2DWindow() = default;

	LRESULT Sandbox2DWindow::MessageHandler
	(
		HWND hWnd,
		UINT message,
		WPARAM wParam,
		LPARAM lParam
	)	
	{
        switch (message)
        {
        case WM_SIZE:
        {
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            if (height == 0) height = 1;
            ::glViewport(0, 0, width, height);
        }
        return 0;

        case WM_KEYDOWN:
            if (wParam == VK_ESCAPE)
            {
                ::DestroyWindow(hWnd);
                return 0;
            }
            break;
        }

        return Window::MessageHandler(hWnd, message, wParam, lParam);
	}
}