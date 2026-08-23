#pragma once
#include "ISessionContext.h"
#include "NetIOCPEvent.h"
#include "RecvBuffer.h"

namespace MGSL::Net
{
	MGSL_CLASS_PTR(SendBuffer)

	class SessionBuffer
	{
		MGSL_DISABLE_COPY(SessionBuffer)
		MGSL_DISABLE_MOVE(SessionBuffer)

	public:
		using OnRecvFunc = Shared::Func<Shared::int32, BYTE*, Shared::int32>;
		using OnSendAction = Shared::Action<Shared::int32>;

		SessionBuffer(ISessionContext& owner, OnRecvFunc onRecv, OnSendAction onSend);
		~SessionBuffer();

	public:
		void Send(SendBufferPtr sendBuffer);

		void RegisterRecv();
		void RegisterSend();

		void ProcessRecv(Shared::int32 numOfBytes);
		void ProcessSend(Shared::int32 numOfBytes);

	public:
		RecvBuffer& GetRecvBuffer() { return m_recvBuffer; }

	private:
		ISessionContext& m_owner;
		OnRecvFunc m_onRecv;
		OnSendAction  m_onSend;

		// RecvBuffer
		RecvBuffer m_recvBuffer{ 0x10000 };

		// SendBuffer
		Shared::Mutex				  m_sendQueueLock;
		Shared::Queue<SendBufferPtr>  m_sendQueue;
		Shared::Atomic<bool>		  m_sendRegistered = false;

	private:
		NetIOCPEvent m_recvEvent{ NetIOCPEventType::Recv };
		NetIOCPEvent m_sendEvent{ NetIOCPEventType::Send };
	};
}

