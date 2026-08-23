#include "ServerLibPch.h"
#include "ServerService.h"
#include "Listener.h"

namespace MGSL::Net
{
	ServerService::ServerService
	(
		NetAddress targetAddress,
		NetIOCPCorePtr core,
		SessionFactory factory,
		Shared::int32 maxSessionCount
	) : Super(NetServiceType::Server, targetAddress, core, factory, maxSessionCount) { }
	ServerService::~ServerService() = default;

	bool ServerService::Start()
	{
		// 0. SessionPool이 정상적으로 생성이 되었는지 검사
		if (!IsReadyToStart()) return false;

		// 1. Listener 생성
		m_listener = std::make_shared<Listener>();
		if (!m_listener) return false;

		// 2. ServerService를 Listener에게 주입
		// 현재 shared_from_this는 Service가 상속받으므로, 해당 함수의 결과는 Service에 대한
		// 스마트 포인터가 생성되기에 캐스팅 필요.
		ServerServicePtr service = static_pointer_cast<ServerService>(shared_from_this());
		if (!m_listener->StartAccept(service)) return false;

		return true;
	}

	void ServerService::CloseService()
	{
		// TODO
	}
}