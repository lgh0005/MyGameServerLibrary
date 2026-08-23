#pragma once

namespace MGSL::Net
{
	MGSL_CLASS_PTR(SendBuffer)

	class SendBuffer : public Shared::Self<SendBuffer>
	{
	public:
		SendBuffer(Shared::int32 bufferSize);
		~SendBuffer();

	public:
		BYTE* Buffer() { return m_sendBuffer.data(); }
		Shared::int32 WriteSize() { return m_writeSize; }
		Shared::int32 Capacity() { return static_cast<Shared::int32>(m_sendBuffer.size()); }

	public:
		void CopyData(void* data, Shared::int32 len);
		void Close(Shared::uint32 writeSize);

	private:
		Shared::List<BYTE> m_sendBuffer;
		Shared::int32 m_writeSize = 0;
	};
}

