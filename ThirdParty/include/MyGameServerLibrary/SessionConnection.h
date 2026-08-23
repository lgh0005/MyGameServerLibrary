#pragma once
#include "ISessionContext.h"
#include "ISessionRegistry.h"
#include "NetIOCPEvent.h"

namespace MGSL::Net
{
	class SessionConnection
	{
		MGSL_DISABLE_COPY(SessionConnection)
		MGSL_DISABLE_MOVE(SessionConnection)

	public:
		using OnConnectedAction = Shared::Action<>;
		using OnDisconnectedAction = Shared::Action<>;
		using OnRecvStartAction = Shared::Action<>;

		SessionConnection
		(
			ISessionContext&     owner,
			OnConnectedAction    onConnected,
			OnDisconnectedAction onDisconnected,
			OnRecvStartAction    onRecvStart
		);
		~SessionConnection();

	public:
		bool IsConnected() const { return m_connected; }
		void SetRegistry(ISessionRegistryWPtr registry) { m_registry = registry; }

	public:
		bool Connect();
		void Disconnect(Shared::cstr cause);
		void ProcessConnect();
		void ProcessDisconnect();

	private:
		bool RegisterConnect();
		bool RegisterDisconnect();

	private:
		ISessionContext& m_owner;
		ISessionRegistryWPtr m_registry;
		Shared::Atomic<bool> m_connected = false;

		NetIOCPEvent m_connectEvent{ NetIOCPEventType::Connect };
		NetIOCPEvent m_disconnectEvent{ NetIOCPEventType::Disconnect };

	private:
		OnConnectedAction    m_onConnected;
		OnDisconnectedAction m_onDisconnected;
		OnRecvStartAction    m_onRecvStart;
	};
}

