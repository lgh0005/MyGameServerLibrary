#include "ServerPch.h"
#include "Server.h"

// 전역 서버 인스턴스
MGSL::Server::Server g_Server;

int main()
{
	// 1. 서버 인스턴스 초기화 및 실행
	g_Server.Init(L"127.0.0.1", 7777);
	g_Server.Update();

	// 2. 실행 종료
	g_Server.Clear();

	return 0;
}