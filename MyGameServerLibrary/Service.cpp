#include "ServerLibPch.h"
#include "Service.h"
#include "SessionPool.h"


namespace MGSL::Net
{
	Service::Service
	(
		NetServiceType type,
		NetAddress address,
		NetIOCPCorePtr core,
		SessionFactory factory,
		Shared::int32 maxSessionCount
	) : m_serviceType(type), m_netAddress(address), m_IOCPCore(core) 
	{
		m_sessionPool = std::make_unique<SessionPool>(factory, maxSessionCount);
	}

	Service::~Service() = default;

	bool Service::IsReadyToStart()
	{
		if (!m_sessionPool) return false;
		m_sessionPool->SetOwner(GetService());
		return m_sessionPool->GetOwner() != nullptr;
	}

	void Service::AddSession(SessionPtr session)
	{
		m_sessionPool->AddSession(session);
	}

	void Service::ReleaseSession(SessionPtr session)
	{
		m_sessionPool->ReleaseSession(session);
	}
}