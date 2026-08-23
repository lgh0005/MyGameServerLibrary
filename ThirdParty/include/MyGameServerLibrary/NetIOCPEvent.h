#pragma once

namespace MGSL::Net
{
	MGSL_CLASS_PTR(Session)
	MGSL_CLASS_PTR(SendBuffer)
	MGSL_CLASS_PTR(NetIOCPObject)

	class NetIOCPEvent : public OVERLAPPED
	{
	public:
		NetIOCPEvent(NetIOCPEventType type);
		~NetIOCPEvent();
		static constexpr Shared::usize ACCEPT_ADDRESS_SIZE		 = sizeof(SOCKADDR_IN) + 16;
		static constexpr Shared::usize TOTAL_ACCEPT_ADDRESS_SIZE = ACCEPT_ADDRESS_SIZE * 2;

	public:
		void Init();
		void SetSession(SessionPtr session) { m_session = session; }
		SessionPtr GetSession() const { return m_session; }
		
		void SetOwner(NetIOCPObjectPtr owner) { m_owner = owner; }
		NetIOCPObjectPtr GetOwner() const { return m_owner; }
		
		BYTE* GetSockAddrBuffer() { return m_acceptedSocketAddr; }

		void SetNetIOCPEventType(NetIOCPEventType type) { m_NetIOCPType = type; }
		const NetIOCPEventType& GetNetIOCPEventType() const { return m_NetIOCPType; }
		
		Shared::List<SendBufferPtr>& GetSendBufferList() { return m_sendBuffers; }
		void ClearSendBufferList() { m_sendBuffers.clear(); }

	private:
		NetIOCPEventType m_NetIOCPType;
		SessionPtr m_session = nullptr;
		NetIOCPObjectPtr m_owner = nullptr;

		BYTE m_acceptedSocketAddr[TOTAL_ACCEPT_ADDRESS_SIZE] = { 0 };
		Shared::List<SendBufferPtr> m_sendBuffers;
	};
}
