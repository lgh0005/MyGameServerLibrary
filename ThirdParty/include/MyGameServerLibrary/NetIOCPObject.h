#pragma once

namespace MGSL::Net
{
	MGSL_CLASS_PTR(NetIOCPObject)
	MGSL_CLASS_PTR(NetIOCPEvent)

	class NetIOCPObject : public Shared::Self<NetIOCPObject>
	{
	public:
		NetIOCPObject();
		virtual ~NetIOCPObject();

	public:
		virtual HANDLE GetObjectHandle() = 0;
		virtual void Dispatch
		(
			NetIOCPEvent* iocpEvent,
			Shared::int32 bytes
		) = 0;
	};
}

