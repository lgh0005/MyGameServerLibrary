#include "ServerLibPch.h"
#include "SessionBuffer.h"
#include "SendBuffer.h"
#include "Session.h"

namespace MGSL::Net
{
	SessionBuffer::SessionBuffer(ISessionContext& owner, OnRecvFunc onRecv, OnSendAction onSend)
		: m_owner(owner)
		, m_onRecv(std::move(onRecv))
		, m_onSend(std::move(onSend)) { }
	SessionBuffer::~SessionBuffer() = default;

	void SessionBuffer::Send(SendBufferPtr sendBuffer)
	{
		bool registerSend = false;
		{
			Shared::LockGuard lock(m_sendQueueLock);
			m_sendQueue.push(sendBuffer);
			registerSend = m_sendRegistered.exchange(true) == false;
		}

		if (registerSend)
			RegisterSend();
	}

	void SessionBuffer::RegisterRecv()
	{
		if (!m_owner.IsConnected())
			return;

		m_recvEvent.Init();
		m_recvEvent.SetOwner(m_owner.GetSessionPtr());

		WSABUF wsaBuf;
		wsaBuf.buf = reinterpret_cast<char*>(m_recvBuffer.WritePos());
		wsaBuf.len = m_recvBuffer.FreeSize();

		if (!SocketUtils::AsyncRecv(m_owner.GetSocket(), &wsaBuf, 1, &m_recvEvent))
		{
			const Shared::int32 errorCode = ::WSAGetLastError();
			m_recvEvent.SetOwner(nullptr);
			m_owner.HandleError(errorCode);
		}
	}

	void SessionBuffer::RegisterSend()
	{
		if (!m_owner.IsConnected())
			return;

		m_sendEvent.Init();
		m_sendEvent.SetOwner(m_owner.GetSessionPtr());

		{
			Shared::LockGuard lock(m_sendQueueLock);

			Shared::int32 writeSize = 0;
			while (m_sendQueue.empty() == false)
			{
				SendBufferPtr sendBuffer = m_sendQueue.front();
				writeSize += sendBuffer->WriteSize();

				// TODO : 예외 체크

				m_sendQueue.pop();
				m_sendEvent.GetSendBufferList().push_back(sendBuffer);
			}
		}

		Shared::List<WSABUF> wsaBufs;
		wsaBufs.reserve(m_sendEvent.GetSendBufferList().size());
		for (SendBufferPtr sendBuffer : m_sendEvent.GetSendBufferList())
		{
			WSABUF wsaBuf;
			wsaBuf.buf = reinterpret_cast<char*>(sendBuffer->Buffer());
			wsaBuf.len = static_cast<ULONG>(sendBuffer->WriteSize());
			wsaBufs.push_back(wsaBuf);
		}

		if (!SocketUtils::AsyncSend(m_owner.GetSocket(), wsaBufs.data(), static_cast<Shared::int32>(wsaBufs.size()), &m_sendEvent))
		{
			Shared::int32 errorCode = ::WSAGetLastError();

			m_sendEvent.SetOwner(nullptr);
			m_sendEvent.ClearSendBufferList();
			m_sendRegistered.store(false);

			m_owner.HandleError(errorCode);
		}
	}

	void SessionBuffer::ProcessRecv(Shared::int32 numOfBytes)
	{
		if (!m_recvEvent.GetOwner()) 
			return;

		if (numOfBytes == 0)
		{
			m_owner.Disconnect("Recv 0");
			return;
		}

		if (!m_recvBuffer.OnWrite(numOfBytes))
		{
			m_owner.Disconnect("OnWrite Overflow");
			return;
		}

		Shared::int32 dataSize = m_recvBuffer.DataSize();
		Shared::int32 processLen = m_onRecv(m_recvBuffer.ReadPos(), dataSize);
		if (processLen < 0 ||
			dataSize < processLen ||
			m_recvBuffer.OnRead(processLen) == false)
		{
			m_owner.Disconnect("OnRead Overflow");
			return;
		}

		m_recvBuffer.Clean();

		RegisterRecv();
	}

	void SessionBuffer::ProcessSend(Shared::int32 numOfBytes)
	{
		m_sendEvent.SetOwner(nullptr);
		m_sendEvent.ClearSendBufferList();

		if (numOfBytes == 0)
		{
			m_owner.Disconnect("Send 0");
			return;
		}

		m_onSend(numOfBytes);

		bool registerSend = false;
		{
			Shared::LockGuard lock(m_sendQueueLock);
			if (m_sendQueue.empty()) m_sendRegistered.store(false);
			else registerSend = true;
		}

		if (registerSend)
			RegisterSend();
	}

}