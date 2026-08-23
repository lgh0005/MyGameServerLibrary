#pragma once
#include "MyGameFramework/Window.h"

namespace MGSL::Sandbox2D
{
	class Sandbox2DWindow : public MGSL::Framework::Window
	{
	public:
		Sandbox2DWindow();
		virtual ~Sandbox2DWindow();

	protected:
		LRESULT MessageHandler
		(
			HWND hWnd,
			UINT message,
			WPARAM wParam,
			LPARAM lParam
		) override;
	};
}

