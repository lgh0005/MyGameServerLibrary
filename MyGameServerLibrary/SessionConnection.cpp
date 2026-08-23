#include "ServerLibPch.h"
#include "SessionConnection.h"
#include "Session.h"

namespace MGSL::Net
{
	SessionConnection::SessionConnection
	(
		ISessionContext&     owner,
		OnConnectedAction    onConnected,
		OnDisconnectedAction onDisconnected,
		OnRecvStartAction    onRecvStart
	)
		: m_owner(owner)
		, m_onConnected(std::move(onConnected))
		, m_onDisconnected(std::move(onDisconnected))
		, m_onRecvStart(std::move(onRecvStart)) { }

	SessionConnection::~SessionConnection() = default;

	bool SessionConnection::Connect()
	{
		return RegisterConnect();
	}

	void SessionConnection::Disconnect(Shared::cstr cause)
	{
		if (m_connected.exchange(false) == false)
			return;

		MGSL_LOG_INFO("Disconnect : {}", cause);

		m_onDisconnected();

		auto registry = m_registry.lock();
		if (registry) registry->ReleaseSession(m_owner.GetSessionPtr());

		RegisterDisconnect();
	}

	void SessionConnection::ProcessConnect()
	{
		m_connectEvent.SetOwner(nullptr);
		m_connected.store(true);

		auto registry = m_registry.lock();
		if (registry) registry->AddSession(m_owner.GetSessionPtr());

		m_onConnected();
		m_onRecvStart();
	}

	void SessionConnection::ProcessDisconnect()
	{
		m_connectEvent.SetOwner(nullptr);
	}

	bool SessionConnection::RegisterConnect()
	{
		if (IsConnected())
			return false;

		auto registry = m_registry.lock();
		if (!registry || registry->GetServiceType() != NetServiceType::Client)
			return false;

		if (!SocketUtils::SetReuseAddress(m_owner.GetSocket(), true)) return false;
		if (!SocketUtils::BindAnyAddress(m_owner.GetSocket(), NetAddress(SocketUtils::ANY_ADDR, 0))) return false;

		m_connectEvent.Init();
		m_connectEvent.SetOwner(m_owner.GetSessionPtr());

		if (!SocketUtils::AsyncConnect(m_owner.GetSocket(), registry->GetNetAddress(), &m_connectEvent))
		{
			const Shared::int32 errorCode = ::WSAGetLastError();
			m_connectEvent.SetOwner(nullptr);
			m_owner.HandleError(errorCode);
			return false;
		}

		return true;
	}

	bool SessionConnection::RegisterDisconnect()
	{
		m_disconnectEvent.Init();
		m_disconnectEvent.SetOwner(m_owner.GetSessionPtr());

		if (!SocketUtils::AsyncDisconnect(m_owner.GetSocket(), &m_disconnectEvent, TF_REUSE_SOCKET))
		{
			const Shared::int32 errorCode = ::WSAGetLastError();
			m_disconnectEvent.SetOwner(nullptr);
			return false;
		}

		return true;
	}
}