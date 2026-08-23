#pragma once

namespace MGSL::Net
{
	MGSL_CLASS_PTR(Session)

	class ISessionContext
	{
		MGSL_DISABLE_COPY(ISessionContext)
		MGSL_DISABLE_MOVE(ISessionContext)

	public:
		ISessionContext();
		virtual ~ISessionContext();

	public:
		virtual SOCKET     GetSocket() const				    = 0;
		virtual bool       IsConnected() const					= 0;
		virtual void	   Disconnect(Shared::cstr cause)       = 0;
		virtual void       HandleError(Shared::int32 errorCode) = 0;
		virtual SessionPtr GetSessionPtr()						= 0;
	};
}

