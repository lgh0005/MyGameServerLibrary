#include "ServerLibPch.h"
#include "Session.h"
#include "Service.h"

namespace MGSL::Net
{
	Session::Session()
	{
		m_socket = SocketUtils::CreateSocket();
	}
	Session::~Session()
	{
		SocketUtils::Close(m_socket);
	}

	HANDLE Session::GetObjectHandle()
	{
		return reinterpret_cast<HANDLE>(m_socket);
	}

	void Session::Dispatch(NetIOCPEvent* iocpEvent, Shared::int32 numOfBytes)
	{
		switch (iocpEvent->GetNetIOCPEventType())
		{
		case NetIOCPEventType::Connect:
			m_connection.ProcessConnect();
			break;
		case NetIOCPEventType::Disconnect:
			m_connection.ProcessDisconnect();
			break;
		case NetIOCPEventType::Recv:
			m_buffer.ProcessRecv(numOfBytes);
			break;
		case NetIOCPEventType::Send:
			m_buffer.ProcessSend(numOfBytes);
			break;
		default:
			break;
		}
	}

	void Session::HandleError(Shared::int32 errorCode)
	{
		switch (errorCode)
		{
		case WSAECONNRESET:
		case WSAECONNABORTED:
			m_connection.Disconnect("HandleError");
			break;

		default:
			MGSL_LOG_WARN("Handle Error : {}", errorCode);
			break;
		}
	}

	void Session::SetService(ServicePtr service)
	{
		m_servcie = service;
		m_connection.SetRegistry(service);
	}
}
