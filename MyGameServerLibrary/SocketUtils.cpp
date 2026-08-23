#include "ServerLibPch.h"
#include "SocketUtils.h"
#include "NetIOCPEvent.h"

namespace MGSL::Net
{
	LPFN_CONNECTEX		SocketUtils::ConnectEx = nullptr;
	LPFN_DISCONNECTEX	SocketUtils::DisconnectEx = nullptr;
	LPFN_ACCEPTEX		SocketUtils::AcceptEx = nullptr;

	SocketUtils::SocketUtils() = default;
	SocketUtils::~SocketUtils() = default;

	/*=======================================================//
	//   default socket initialization and closing methods   //
	//=======================================================*/
	void SocketUtils::Init()
	{
		// 0. winsock 초기화
		WSADATA wsaData;
		MGSL_ASSERT(::WSAStartup(MAKEWORD(2, 2), OUT & wsaData) == 0);

		// 1. AcceptEx, ConnectEx, DisconnectEx를 위한 런타임 함수 포인터 설정
		SOCKET dummySocket = CreateSocket();
		MGSL_ASSERT(BindAsyncSocketFunction(dummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)));
		MGSL_ASSERT(BindAsyncSocketFunction(dummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)));
		MGSL_ASSERT(BindAsyncSocketFunction(dummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)));
		Close(dummySocket);
	}

	void SocketUtils::Clear()
	{
		::WSACleanup();
	}

	SOCKET SocketUtils::CreateSocket()
	{
		return ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	}

	void SocketUtils::Close(SOCKET& socket)
	{
		if (socket != INVALID_SOCKET) ::closesocket(socket);
		socket = INVALID_SOCKET;
	}

	bool SocketUtils::BindAsyncSocketFunction(SOCKET socket, GUID guid, LPVOID* fn)
	{
		DWORD bytes = 0;
		return SOCKET_ERROR != ::WSAIoctl
		(
			socket, 
			SIO_GET_EXTENSION_FUNCTION_POINTER, 
			&guid, 
			sizeof(guid), 
			fn, 
			sizeof(*fn), 
			&bytes, 
			nullptr, 
			nullptr
		);
	}

	/*===========================================//
	//   default socket option setting methods   //
	//===========================================*/
	bool SocketUtils::SetLinger(SOCKET socket, Shared::uint16 onoff, Shared::uint16 linger)
	{
		LINGER option;
		option.l_onoff = onoff;
		option.l_linger = linger;
		return SetSockOpt(socket, SOL_SOCKET, SO_LINGER, option);
	}

	bool SocketUtils::SetReuseAddress(SOCKET socket, bool flag)
	{
		return SetSockOpt(socket, SOL_SOCKET, SO_REUSEADDR, flag);
	}

	bool SocketUtils::SetRecvBufferSize(SOCKET socket, Shared::int32 size)
	{
		return SetSockOpt(socket, SOL_SOCKET, SO_RCVBUF, size);
	}

	bool SocketUtils::SetSendBufferSize(SOCKET socket, Shared::int32 size)
	{
		return SetSockOpt(socket, SOL_SOCKET, SO_SNDBUF, size);
	}

	bool SocketUtils::SetTcpNoDelay(SOCKET socket, bool flag)
	{
		return SetSockOpt(socket, SOL_SOCKET, TCP_NODELAY, flag);
	}

	bool SocketUtils::SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket)
	{
		return SetSockOpt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listenSocket);
	}
	
	/*===================================//
	//   default listen socket methods   //
	//===================================*/
	bool SocketUtils::Bind(SOCKET socket, NetAddress netAddr)
	{
		return ::bind
		(
			socket, 
			reinterpret_cast<const SOCKADDR*>(&netAddr.GetSockAddr()), 
			sizeof(SOCKADDR_IN)
		) != SOCKET_ERROR;
	}

	bool SocketUtils::BindAnyAddress(SOCKET socket, NetAddress netAddr)
	{
		NetAddress anyAddr(ANY_ADDR, netAddr.GetPort());
		return Bind(socket, anyAddr);
	}

	bool SocketUtils::Listen(SOCKET socket, Shared::int32 backlog)
	{
		return ::listen(socket, backlog) != SOCKET_ERROR;
	}

	/*================================================//
	//   default async i/o request wrapping methods   //
	//================================================*/
	bool SocketUtils::AsyncRecv(SOCKET socket, WSABUF* wsaBufs, Shared::int32 bufCount, NetIOCPEvent* iocpEvent)
	{
		DWORD numOfBytes = 0;
		DWORD flags = 0;
		const Shared::int32 result = ::WSARecv(socket, wsaBufs, static_cast<DWORD>(bufCount), &numOfBytes, &flags, iocpEvent, nullptr);

		if (result != SOCKET_ERROR)
			return true;

		return ::WSAGetLastError() == WSA_IO_PENDING;
	}

	bool SocketUtils::AsyncSend(SOCKET socket, WSABUF* wsaBufs, Shared::int32 bufCount, NetIOCPEvent* iocpEvent)
	{
		DWORD numOfBytes = 0;
		const Shared::int32 result = ::WSASend(socket, wsaBufs, static_cast<DWORD>(bufCount), &numOfBytes, 0, iocpEvent, nullptr);

		if (result != SOCKET_ERROR)
			return true;

		return ::WSAGetLastError() == WSA_IO_PENDING;
	}

	bool SocketUtils::AsyncConnect(SOCKET socket, NetAddress address, NetIOCPEvent* iocpEvent)
	{
		MGSL_ASSERT(ConnectEx != nullptr);

		DWORD numOfBytes = 0;
		SOCKADDR_IN sockAddr = address.GetSockAddr();
		const BOOL result = SocketUtils::ConnectEx(socket, reinterpret_cast<SOCKADDR*>(&sockAddr), sizeof(sockAddr), nullptr, 0, &numOfBytes, iocpEvent);

		if (result == TRUE)
			return true;

		return ::WSAGetLastError() == WSA_IO_PENDING;
	}

	bool SocketUtils::AsyncDisconnect(SOCKET socket, NetIOCPEvent* iocpEvent, DWORD flags)
	{
		MGSL_ASSERT(DisconnectEx != nullptr);

		const BOOL result = SocketUtils::DisconnectEx(socket, iocpEvent, flags, 0);
		if (result == TRUE)
			return true;

		return ::WSAGetLastError() == WSA_IO_PENDING;
	}

	bool SocketUtils::AsyncAccept(SOCKET listenSocket, SOCKET acceptSocket, BYTE* addressBuffer, NetIOCPEvent* iocpEvent)
	{
		MGSL_ASSERT(AcceptEx != nullptr);

		DWORD numOfBytes = 0;
		const DWORD addressLen = NetIOCPEvent::ACCEPT_ADDRESS_SIZE;

		const BOOL result = SocketUtils::AcceptEx(listenSocket, acceptSocket, addressBuffer, 0, addressLen, addressLen, &numOfBytes, iocpEvent);
		if (result == TRUE)
			return true;

		return ::WSAGetLastError() == WSA_IO_PENDING;
	}
}