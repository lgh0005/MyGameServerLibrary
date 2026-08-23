#include "2DPch.h"
#include "Sandbox2DWindow.h"
#include "Sandbox2D.h"

// 전역 게임 인스턴스
MGSL::Sandbox2D::Sandbox2D g_Game2D;

MGSL::Shared::int32 APIENTRY wWinMain
(
    _In_ HINSTANCE      hInstance,
    _In_opt_ HINSTANCE  hPrevInstance,
    _In_ LPWSTR         lpCmdLine,
    _In_ MGSL::Shared::int32          nCmdShow
)
{
    // 0. 실행 시 뜰 윈도우 설정
    MGSL::Framework::WindowDesc windowDesc = 
    {
        L"MGSL_WindowClass2D",
        L"MySandbox2D",
        1280,
        720,
        false
    };

    // 1. 게임 인스턴스 생성 및 실행
    g_Game2D.Init(windowDesc);
    g_Game2D.Run();

    // 2. 실행 종료
    g_Game2D.Clear();

    return 0;
}