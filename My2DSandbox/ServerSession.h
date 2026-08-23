#pragma once
#include "MyGameServerLibrary/PacketSession.h"

namespace MGSL::Net
{
	class ServerSession : public PacketSession
	{
	public:
		ServerSession();
		virtual ~ServerSession() override;

	public:
		virtual void OnConnected() override;
		virtual void OnDisconnected() override;
		virtual void OnRecvPacket(BYTE* buffer, Shared::int32 len) override;
		virtual void OnSend(Shared::int32 len) override;

	private:
		ServerSessionPtr GetServerSession();
	};
}

