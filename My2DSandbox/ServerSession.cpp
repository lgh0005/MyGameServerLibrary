#include "2DPch.h"
#include "ServerSession.h"
#include "ClientPacketHandler.h"

namespace MGSL::Net
{
	ServerSession::ServerSession() = default;
	ServerSession::~ServerSession() = default;

	void ServerSession::OnConnected()
	{
		MGSL_LOG_INFO("Connected to Server.");
	}

	void ServerSession::OnDisconnected()
	{
		MGSL_LOG_INFO("Disconnected to Server.");
	}

	void ServerSession::OnRecvPacket(BYTE* buffer, Shared::int32 len)
	{
		ClientPacketHandler::HandlePacket(GetServerSession(), buffer, len);
	}

	void ServerSession::OnSend(Shared::int32 len)
	{

	}

	ServerSessionPtr ServerSession::GetServerSession()
	{
		return static_pointer_cast<ServerSession>(shared_from_this());
	}
}