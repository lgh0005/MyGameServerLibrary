#include "ServerLibPch.h"
#include "Listener.h"
#include "NetIOCPCore.h"
#include "NetIOCPEvent.h"
#include "Session.h"
#include "NetAddress.h"
#include "ServerService.h"
#include "SessionPool.h"

namespace MGSL::Net
{
	Listener::Listener() = default;
	Listener::~Listener() = default;

	void Listener::CloseSocket()
	{
		SocketUtils::Close(m_listenSocket);
	}

	HANDLE Listener::GetObjectHandle()
	{
		return reinterpret_cast<HANDLE>(m_listenSocket);
	}

	void Listener::Dispatch(NetIOCPEvent* acceptEvent, Shared::int32 numOfBytes)
	{
		MGSL_ASSERT(acceptEvent->GetNetIOCPEventType() == NetIOCPEventType::Accept);
		ProcessAccept(acceptEvent);
	}

	bool Listener::StartAccept(ServerServicePtr service)
	{
		// 0. ServerService를 weak_ptr로 소유
		m_serverService = service;
		if (m_serverService.expired()) return false;

		// 1. Listen 소켓 생성
		m_listenSocket = SocketUtils::CreateSocket();
		if (m_listenSocket == INVALID_SOCKET)
			return false;

		// 2. Listener 자신을 곧바로 등록
		// AcceptEx를 위해서 IOCP 큐의 관찰 대상이 되어야 함
		if (!GetServerService()->GetIOCPCore()->Register(shared_from_this()))
			return false;

		// 3. Listen 소켓에 ResuseAddress와 Linger 옵션 설정
		if (!SocketUtils::SetReuseAddress(m_listenSocket, true)) return false;
		if (!SocketUtils::SetLinger(m_listenSocket, 0, 0)) return false;

		// 4. Bind 및 Listen
		auto serverService = m_serverService.lock();
		if (!SocketUtils::Bind(m_listenSocket, serverService->GetNetAddress())) return false;
		if (!SocketUtils::Listen(m_listenSocket)) return false;

		// 5. ServerService가 들고 있는 최대 세션 개수만큼 순회하여
		// Accept 요청 등록
		const Shared::int32 acceptCount = serverService->GetSessionPool()->GetMaxSessionCount();
		for (Shared::int32 i = 0; i < acceptCount; i++)
		{
			NetIOCPEvent* acceptEvent = new NetIOCPEvent(NetIOCPEventType::Accept);
			acceptEvent->SetOwner(shared_from_this());
			m_acceptEvents.push_back(acceptEvent);
			RegisterAccept(acceptEvent);
		}

		return true;
	}

	SOCKADDR_IN Listener::GetAcceptedSocketAddress(NetIOCPEvent* acceptEvent)
	{
		if (acceptEvent == nullptr)
			return {};

		SessionPtr session = acceptEvent->GetSession();
		if (session == nullptr || session->GetSocket() == INVALID_SOCKET)
			return {};

		SOCKADDR_IN sockAddress{};
		Shared::int32 sizeOfSockAddr = sizeof(sockAddress);
		if (::getpeername
		(
			session->GetSocket(),
			reinterpret_cast<SOCKADDR*>(&sockAddress),
			&sizeOfSockAddr
		) == SOCKET_ERROR)
		{
			const Shared::int32 errorCode = ::WSAGetLastError();

			MGSL_LOG_ERROR
			(
				"getpeername failed. error={}, socket={}",
				errorCode,
				static_cast<Shared::uint64>(session->GetSocket())
			);

			return {};
		}

		return sockAddress;
	}

	void Listener::ProcessAccept(NetIOCPEvent* acceptEvent)
	{
		// 1. 해당 NetIOCPEvent로부터 세션을 획득
		SessionPtr session = acceptEvent->GetSession();

		// 2. 세션에 있는 클라이언트 소켓에 SO_UPDATE_ACCEPT_CONTEXT 옵션 설정
		if (!SocketUtils::SetUpdateAcceptSocket(session->GetSocket(), m_listenSocket))
		{
			// 소켓 옵션 설정에 실패하면 다시 Accept를 시도
			RegisterAccept(acceptEvent);
			return;
		}

		// 3. 클라이언트로부터 IP 주소 받아오기
		SOCKADDR_IN sockAddr = GetAcceptedSocketAddress(acceptEvent);
		if (sockAddr.sin_family == AF_UNSPEC)
		{
			RegisterAccept(acceptEvent);
			return;
		}

		// 4. 세션의 NetAddress 설정
		NetAddress netAddress(sockAddr);
		session->SetNetAddress(netAddress);

		// 5. Connect 요청
		session->GetSessionConnection().ProcessConnect();

		// 6. 최종 입출력 요청을 등록
		RegisterAccept(acceptEvent);
	}

	void Listener::RegisterAccept(NetIOCPEvent* acceptEvent)
	{
		// 1. 접속된 클라이언트에 대한 세션 생성
		auto serverService = m_serverService.lock();
		SessionPtr session = serverService->GetSessionPool()->CreateSession();

		// 2. acceptEvent를 먼저 초기화 후, 생성한 세션 연결
		acceptEvent->Init();
		acceptEvent->SetSession(session);

		// 3. 비동기 Accept 수행
		DWORD bytesReceived = 0;
		if (!SocketUtils::AcceptEx
			(
				m_listenSocket,
				session->GetSocket(),
				acceptEvent->GetSockAddrBuffer(),
				0,
				NetIOCPEvent::ACCEPT_ADDRESS_SIZE,
				NetIOCPEvent::ACCEPT_ADDRESS_SIZE,
				&bytesReceived,
				static_cast<LPOVERLAPPED>(acceptEvent)
			)
		)
		{
			const Shared::int32 errorCode = ::GetLastError();
			if (errorCode != WSA_IO_PENDING)
				RegisterAccept(acceptEvent);
		}
	}
}
