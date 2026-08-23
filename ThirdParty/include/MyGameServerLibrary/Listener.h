#pragma once
#include "NetIOCPObject.h"

namespace MGSL::Net
{
	MGSL_CLASS_PTR(NetIOCPEvent)
	MGSL_CLASS_PTR(ServerService)
	MGSL_CLASS_PTR(Listener)

	class Listener : public NetIOCPObject
	{
	public:
		Listener();
		virtual ~Listener() override;

	public:
		bool StartAccept(ServerServicePtr service);
		void CloseSocket();

	public:
		ServerServicePtr GetServerService() { return m_serverService.lock(); }

	public:
		virtual HANDLE	GetObjectHandle() override;
		virtual void	Dispatch
		(
			NetIOCPEvent* iocpEvent,
			Shared::int32 numOfBytes = 0
		) override;

	private:
		SOCKADDR_IN GetAcceptedSocketAddress(NetIOCPEvent* acceptEvent);
		void ProcessAccept(NetIOCPEvent* acceptEvent);
		void RegisterAccept(NetIOCPEvent* acceptEvent);

	private:
		SOCKET m_listenSocket = INVALID_SOCKET;
		Shared::List<NetIOCPEvent*> m_acceptEvents;
		ServerServiceWPtr m_serverService;
	};
}

