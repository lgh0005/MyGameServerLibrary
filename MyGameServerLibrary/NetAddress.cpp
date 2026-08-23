#include "ServerLibPch.h"
#include "NetAddress.h"

namespace MGSL::Net
{
	NetAddress::NetAddress() = default;
	NetAddress::NetAddress(SOCKADDR_IN sockAddr) : m_sockAddr(sockAddr) { }
	NetAddress::NetAddress(Shared::WString ip, Shared::uint16 port)
	{
		::ZeroMemory(&m_sockAddr, sizeof(m_sockAddr));
		m_sockAddr.sin_family = AF_INET;
		m_sockAddr.sin_addr = IPtoAddr(ip.c_str());
		m_sockAddr.sin_port = ::htons(port);
	}
	NetAddress::~NetAddress() = default;

	Shared::WString NetAddress::GetIPAddress()
	{
		WCHAR buffer[INET_ADDRSTRLEN];
		::InetNtopW(AF_INET, &m_sockAddr.sin_addr, buffer, std::size(buffer));
		return Shared::WString(buffer);
	}

	Shared::uint16 NetAddress::GetPort()
	{
		return ::ntohs(m_sockAddr.sin_port);
	}

	IN_ADDR NetAddress::IPtoAddr(const WCHAR* ip)
	{
		IN_ADDR address;
		::InetPtonW(AF_INET, ip, &address);
		return address;
	}
}
