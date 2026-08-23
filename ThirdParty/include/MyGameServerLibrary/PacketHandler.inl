#pragma once
#include "SendBuffer.h"

namespace MGSL::Net
{
	template<typename T> PacketHandler<T>::PacketHandler() = default;
	template<typename T> PacketHandler<T>::~PacketHandler() = default;

	template<typename T>
	void PacketHandler<T>::HandlePacket(PacketSessionPtr session, BYTE* buffer, Shared::int32 len)
	{
		T::HandlePacketImpl(session, buffer, len);
	}

	template<typename T>
	template<typename PacketType>
	SendBufferPtr PacketHandler<T>::MakeSendBuffer
	(
		PacketType& pkt,
		Shared::uint16 packetID
	)
	{
		const Shared::uint16 dataSize = static_cast<Shared::uint16>(pkt.ByteSizeLong());
		const Shared::uint16 packetSize = dataSize + sizeof(Protocol::PacketHeader);

		SendBufferPtr sendBuffer = std::make_shared<Net::SendBuffer>(packetSize);
		Protocol::PacketHeader* header = reinterpret_cast<Protocol::PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = packetID;

		pkt.SerializeToArray(&header[1], dataSize);
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
}