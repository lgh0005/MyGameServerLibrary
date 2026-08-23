#pragma once
#include "NetAddress.h"

namespace MGSL::Net
{
	MGSL_CLASS_PTR(NetIOCPEvent)

	class SocketUtils
	{
		MGSL_DISABLE_COPY(SocketUtils)
		MGSL_DISABLE_MOVE(SocketUtils)

	public:
		static constexpr Shared::wcstr ANY_ADDR = L"0.0.0.0";

	private:
		SocketUtils();
		~SocketUtils();

	/*=======================================================//
	//   default socket initialization and closing methods   //
	//=======================================================*/
	public:
		static void Init();
		static void Clear();
		static SOCKET CreateSocket();
		static void Close(SOCKET& socket);

	/*===================================//
	//   default listen socket methods   //
	//===================================*/
	public:
		static bool Bind(SOCKET socket, NetAddress netAddr);
		static bool BindAnyAddress(SOCKET socket, NetAddress netAddr);
		static bool Listen(SOCKET socket, Shared::int32 backlog = SOMAXCONN);

	/*===========================================//
	//   default socket option setting methods   //
	//===========================================*/
	public:
		static bool SetLinger(SOCKET socket, Shared::uint16 onoff, Shared::uint16 linger);
		static bool SetReuseAddress(SOCKET socket, bool flag);
		static bool SetRecvBufferSize(SOCKET socket, Shared::int32 size);
		static bool SetSendBufferSize(SOCKET socket, Shared::int32 size);
		static bool SetTcpNoDelay(SOCKET socket, bool flag);
		static bool SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket);

	/*================================================//
	//   default async i/o request wrapping methods   //
	//================================================*/
	public:
		static bool AsyncRecv(SOCKET socket, WSABUF* wsaBufs, Shared::int32 bufCount, NetIOCPEvent* iocpEvent);
		static bool AsyncSend(SOCKET socket, WSABUF* wsaBufs, Shared::int32 bufCount, NetIOCPEvent* iocpEvent);
		static bool AsyncConnect(SOCKET socket, NetAddress address, NetIOCPEvent* iocpEvent);
		static bool AsyncDisconnect(SOCKET socket, NetIOCPEvent* iocpEvent, DWORD flags = TF_REUSE_SOCKET);
		static bool AsyncAccept(SOCKET listenSocket, SOCKET acceptSocket, BYTE* addressBuffer, NetIOCPEvent* iocpEvent);

	/*==================================================//
	//   default utilities for registers and settings   //
	//==================================================*/
	private:
		template<typename T>
		bool SetSocketOption
		(
			SOCKET socket,
			Shared::int32 level,
			Shared::int32 optName,
			T optVal
		);

		static bool BindAsyncSocketFunction
		(
			SOCKET socket, 
			GUID guid, 
			LPVOID* fn
		);

	public:
		static LPFN_CONNECTEX		ConnectEx;
		static LPFN_DISCONNECTEX	DisconnectEx;
		static LPFN_ACCEPTEX		AcceptEx;
	};
}

#include "SocketUtils.inl"