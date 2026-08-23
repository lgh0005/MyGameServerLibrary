#pragma once

namespace MGSL::Framework
{
	class GLContext
	{
	public:
		GLContext();
		~GLContext();

	public:
		bool Init(HWND hWnd);
		void Clear(HWND hWnd);

		// TODO : 이 메서드는 카메라가 들고 있어야 할 영역
		void SetClearColor(float r, float g, float b, float a = 1.0f);

	public:
		void SwapBuffers();
		void ClearBuffer();

	private:
		void MakeCurrent();
		void ResetCurrent();

	private:
		HDC   m_hDC = nullptr;
		HGLRC m_hRC = nullptr;
	};
}

