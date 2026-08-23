#include "ServerLibPch.h"
#include "SendBuffer.h"

namespace MGSL::Net
{
	SendBuffer::SendBuffer(Shared::int32 bufferSize)
	{
		m_sendBuffer.resize(bufferSize);
	}
	SendBuffer::~SendBuffer() = default;

	void SendBuffer::CopyData(void* data, Shared::int32 len)
	{
		MGSL_ASSERT(Capacity() >= len);
		::CopyMemory(m_sendBuffer.data(), data, len);
		m_writeSize = len;
	}

	void SendBuffer::Close(Shared::uint32 writeSize)
	{
		m_writeSize = writeSize;
	}
}