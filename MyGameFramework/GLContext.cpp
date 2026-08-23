#include "GameFrameworkPch.h"
#include "GLContext.h"

namespace MGSL::Framework
{
    GLContext::GLContext() = default;
    GLContext::~GLContext() = default;

    bool GLContext::Init(HWND hWnd)
    {
        m_hDC = ::GetDC(hWnd);
        if (!m_hDC) return false;

        PIXELFORMATDESCRIPTOR pfd = {};
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;
        pfd.iLayerType = PFD_MAIN_PLANE;

        Shared::int32 pixelFormat = ::ChoosePixelFormat(m_hDC, &pfd);
        if (pixelFormat == 0) return false;

        if (!::SetPixelFormat(m_hDC, pixelFormat, &pfd)) return false;

        m_hRC = ::wglCreateContext(m_hDC);
        if (!m_hRC) return false;

        if (!::wglMakeCurrent(m_hDC, m_hRC)) return false;

        // OpenGL glew 함수 포인터 초기화
        glewExperimental = GL_TRUE;
        const GLenum glewResult = ::glewInit();
        if (glewResult != GLEW_OK) return false;

        // VSync OFF
        if (WGLEW_EXT_swap_control)
            ::wglSwapIntervalEXT(0);

        /// 기본 OpenGL 상태
        ::glEnable(GL_DEPTH_TEST);
        ::glDepthFunc(GL_LESS);
        ::glDepthMask(GL_TRUE);
        ::glClearDepth(1.0);
        ::glEnable(GL_BLEND);
        ::glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        return true;
    }

    void GLContext::Clear(HWND hWnd)
    {
        if (m_hRC)
        {
            ResetCurrent();
            ::wglDeleteContext(m_hRC);
            m_hRC = nullptr;
        }

        if (m_hDC)
        {
            ::ReleaseDC(hWnd, m_hDC);
            m_hDC = nullptr;
        }
    }

    void GLContext::SwapBuffers()
    {
        if (m_hDC) ::SwapBuffers(m_hDC);
    }

    void GLContext::MakeCurrent()
    {
        if (m_hDC && m_hRC) ::wglMakeCurrent(m_hDC, m_hRC);
    }

    void GLContext::ResetCurrent()
    {
        ::wglMakeCurrent(nullptr, nullptr);
    }

    void GLContext::SetClearColor(float r, float g, float b, float a)
    {
        ::glClearColor(r, g, b, a);
    }

    void GLContext::ClearBuffer()
    {
        ::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}