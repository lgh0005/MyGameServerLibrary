#pragma once

namespace MGSL::Net
{
	MGSL_CLASS_PTR(NetIOCPObject)

	class NetIOCPCore : public Shared::Self<NetIOCPCore>
	{
	public:
		NetIOCPCore();
		~NetIOCPCore();

	public:
		HANDLE GetNetIOCPHandle() const { return m_netIOCPHandle; }
		bool Register(NetIOCPObjectPtr iocpObject);
		bool Dispatch(Shared::uint32 timeout = INFINITE);

	private:
		HANDLE m_netIOCPHandle;
	};
}

