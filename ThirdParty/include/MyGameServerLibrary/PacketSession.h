#pragma once
#include "Session.h"

namespace MGSL::Net
{
	class PacketSession : public Session
	{
	public:
		PacketSession();
		virtual ~PacketSession();

	protected:
		virtual Shared::int32 OnRecv(BYTE* buffer, Shared::int32 len) final;
		virtual void OnRecvPacket(BYTE* buffer, Shared::int32 len) = 0;
	};
}

