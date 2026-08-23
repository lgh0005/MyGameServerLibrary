#include "ServerLibPch.h"
#include "SessionPool.h"
#include "Session.h"
#include "Service.h"
#include "NetIOCPCore.h"

namespace MGSL::Net
{
	SessionPool::SessionPool
	(
		SessionFactory factory,
		Shared::int32 maxSessionCount
	) :	m_sessionFactory(factory), 
		m_maxSessionCount(maxSessionCount) { }
	SessionPool::~SessionPool() = default;

	SessionPtr SessionPool::CreateSession()
	{
		SessionPtr session = m_sessionFactory();

		auto service = m_owner.lock();
		session->SetService(service);
		if (!service->GetIOCPCore()->Register(session))
			return nullptr;

		return session;
	}

	void SessionPool::AddSession(SessionPtr session)
	{
		Shared::LockGuard lock(m_sessionMutex);
		m_sessions.insert(session);
		m_sessionCount++;
	}

	void SessionPool::ReleaseSession(SessionPtr session)
	{
		Shared::LockGuard lock(m_sessionMutex);
		m_sessions.erase(session);
		m_sessionCount--;
	}
}