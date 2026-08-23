#include "ServerLibPch.h"
#include "NetIOCPEvent.h"

namespace MGSL::Net
{
	NetIOCPEvent::NetIOCPEvent(NetIOCPEventType type) : m_NetIOCPType(type)
	{
		Init();
	}
	NetIOCPEvent::~NetIOCPEvent() = default;
 
	void NetIOCPEvent::Init()
	{
		::ZeroMemory(this, sizeof(OVERLAPPED));
	}
}