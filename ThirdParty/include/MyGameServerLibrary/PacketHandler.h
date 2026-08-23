#pragma once

namespace MGSL::Net
{
	MGSL_CLASS_PTR(SendBuffer)
	MGSL_CLASS_PTR(PacketSession)

	template<typename T>
	class PacketHandler
	{
	public:
		static void HandlePacket(PacketSessionPtr session, BYTE* buffer, Shared::int32 len);
		
	protected:
		template<typename PacketType>
		static SendBufferPtr MakeSendBuffer
		(
			PacketType& pkt,
			Shared::uint16 packetID
		);

	protected:
		PacketHandler();
		~PacketHandler();
	};
}

#include "PacketHandler.inl"