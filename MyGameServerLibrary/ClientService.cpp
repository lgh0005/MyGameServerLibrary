#include "ServerLibPch.h"
#include "ClientService.h"
#include "SessionPool.h"
#include "Session.h"

namespace MGSL::Net
{
	ClientService::ClientService
	(
		NetAddress targetAddress,
		NetIOCPCorePtr core,
		SessionFactory factory,
		Shared::int32 maxSessionCount
	) : Super(NetServiceType::Client, targetAddress, core, factory, maxSessionCount) { }
	ClientService::~ClientService() = default;

	bool ClientService::Start()
	{
		// 0. SessionPool이 정상적으로 생성이 되었는지 검사
		if (!IsReadyToStart()) return false;

		// 2. 생성자에서 받았었던 최대 세션 제한 수를 바탕으로 순회
		const Shared::int32 sessionCount = m_sessionPool->GetMaxSessionCount();
		for (Shared::int32 i = 0; i < sessionCount; i++)
		{
			SessionPtr session = m_sessionPool->CreateSession();
			if (!session->Connect())
				return false;
		}

		return true;
	}
}