#pragma once
#include "ISessionRegistry.h"

namespace MGSL::Net
{
	MGSL_CLASS_PTR(Service)
	MGSL_CLASS_PTR(Session)
	MGSL_CLASS_PTR(SessionPool)
	MGSL_CLASS_PTR(NetIOCPCore)

	class Service : public Shared::Self<Service>, public ISessionRegistry
	{
	public:
		using SessionFactory = Shared::Func<SessionPtr>;

	public:
		Service
		(
			NetServiceType type,
			NetAddress address,
			NetIOCPCorePtr core,
			SessionFactory factory,
			Shared::int32 maxSessionCount = 1
		);
		virtual ~Service();

	public:
		virtual bool Start() = 0;
		bool IsReadyToStart();

	/*=======================================//
	//   ISessionRegistry implementations   //
	//=======================================*/
	public:
		virtual NetServiceType GetServiceType() const override { return m_serviceType; }
		virtual NetAddress     GetNetAddress() const override { return m_netAddress; }
		virtual void           AddSession(SessionPtr session) override;
		virtual void           ReleaseSession(SessionPtr session) override;

	public:
		NetIOCPCorePtr& GetIOCPCore()    { return m_IOCPCore; }
		SessionPool* GetSessionPool() const { return m_sessionPool.get(); }
		ServicePtr GetService() { return shared_from_this(); }

	/*=====================//
	//   service members   //
	//=====================*/
	protected:
		NetServiceType	   m_serviceType;
		NetAddress		   m_netAddress;
		NetIOCPCorePtr	   m_IOCPCore;
		SessionPoolUPtr    m_sessionPool;
		SessionFactory     m_sessionFactory;
	};
}

