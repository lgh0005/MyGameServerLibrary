#pragma once

namespace MGSL::Net
{
	MGSL_CLASS_PTR(Session)
	MGSL_CLASS_PTR(ISessionRegistry)

	class ISessionRegistry
	{
		MGSL_DISABLE_COPY(ISessionRegistry)
		MGSL_DISABLE_MOVE(ISessionRegistry)

	public:
		ISessionRegistry();
		virtual ~ISessionRegistry();

	public:
		virtual NetServiceType GetServiceType() const			  = 0;
		virtual NetAddress     GetNetAddress() const			  = 0;
		virtual void           AddSession(SessionPtr session)	  = 0;
		virtual void           ReleaseSession(SessionPtr session) = 0;
	};
}

