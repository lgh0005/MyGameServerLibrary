#pragma once

namespace MGSL::Net
{
	class NetAddress
	{
	public:
		NetAddress();
		NetAddress(SOCKADDR_IN sockAddr);
		NetAddress(Shared::WString ip, Shared::uint16 port);
		~NetAddress();

	public:
		SOCKADDR_IN& GetSockAddr() { return m_sockAddr; }
		Shared::WString GetIPAddress();
		Shared::uint16 GetPort();
		IN_ADDR IPtoAddr(const WCHAR* ip);

	private:
		SOCKADDR_IN m_sockAddr = {};
	};
}

