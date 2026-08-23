#pragma once

namespace MGSL::Net
{
	MGSL_CLASS_PTR(Service)
	MGSL_CLASS_PTR(Session)

	class SessionPool
	{
		using SessionFactory = Shared::Func<SessionPtr>;

	public:
		SessionPool
		(
			SessionFactory factory,
			Shared::int32 maxSessionCount = 1
		);
		~SessionPool();

	public:
		void SetOwner(ServicePtr owner) { m_owner = owner; }
		ServicePtr GetOwner() const { return m_owner.lock(); }

	public:
		SessionPtr		CreateSession();
		void			AddSession(SessionPtr session);
		void			ReleaseSession(SessionPtr session);
		Shared::int32	GetCurrentSessionCount() { return m_sessionCount; }
		Shared::int32	GetMaxSessionCount() { return m_maxSessionCount; }

	private:
		ServiceWPtr m_owner;
		SessionFactory m_sessionFactory;
		Shared::Mutex m_sessionMutex;
		Shared::HashSet<SessionPtr> m_sessions;
		Shared::int32 m_sessionCount	= 0;
		Shared::int32 m_maxSessionCount = 0;
	};
}


