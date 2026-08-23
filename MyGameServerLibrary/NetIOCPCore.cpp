#include "ServerLibPch.h"
#include "NetIOCPCore.h"
#include "NetIOCPObject.h"
#include "NetIOCPEvent.h"

namespace MGSL::Net
{
	NetIOCPCore::NetIOCPCore()
	{
		// 최초 호출되는 CreateIoCompletionPort 함수로 IOCP 큐를 생성
		m_netIOCPHandle = ::CreateIoCompletionPort
		(
			INVALID_HANDLE_VALUE, 
			0, 
			0, 
			0
		);
		MGSL_ASSERT(m_netIOCPHandle != INVALID_HANDLE_VALUE);
	}

	NetIOCPCore::~NetIOCPCore()
	{
		::CloseHandle(m_netIOCPHandle);
	}

	bool NetIOCPCore::Register(NetIOCPObjectPtr iocpObject)
	{
		// 두 번째 호출되는 CreateIoCompletionPort 함수로 IOCP 큐의 관찰 대상으로 등록
		return ::CreateIoCompletionPort
		(
			iocpObject->GetObjectHandle(), 
			m_netIOCPHandle,
			/*key*/0, 
			0
		);
	}

	bool NetIOCPCore::Dispatch(Shared::uint32 timeout)
	{
		DWORD numOfBytes = 0;
		ULONG_PTR key = 0;
		NetIOCPEvent* iocpEvent = nullptr;
	
		if (::GetQueuedCompletionStatus
		(
			m_netIOCPHandle,
			&numOfBytes, 
			reinterpret_cast<PULONG_PTR>(&key), 
			reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), 
			timeout
		))
		{
			// PostQueuedCompletionStatus로 전달된 종료 패킷
			if (iocpEvent == nullptr) return false;

			// IOCP 이벤트를 소유한 객체에게 디스패치
			NetIOCPObjectPtr iocpObject = iocpEvent->GetOwner();
			iocpObject->Dispatch(iocpEvent, numOfBytes);
		}
		else
		{
			Shared::int32 err = ::GetLastError();
			switch (err)
			{
			case WAIT_TIMEOUT:
				return false;
			default:
				NetIOCPObjectPtr iocpObject = iocpEvent->GetOwner();
				iocpObject->Dispatch(iocpEvent, numOfBytes);
				break;
			}
		}

		return true;
	}
}

