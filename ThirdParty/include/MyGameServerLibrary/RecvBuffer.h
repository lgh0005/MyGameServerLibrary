#pragma once

namespace MGSL::Net
{
	class RecvBuffer
	{
	public:
		RecvBuffer(Shared::int32 bufferSize);
		~RecvBuffer();
		static constexpr Shared::usize BUFFER_COUNT = 10;

	public:
		void		  Clean();
		bool		  OnRead(Shared::int32 numOfBytes);
		bool		  OnWrite(Shared::int32 numOfBytes);

	public:
		BYTE*		  ReadPos() { return &m_recvBuffer[m_readPos]; }
		BYTE*		  WritePos() { return &m_recvBuffer[m_writePos]; }
		Shared::int32 DataSize() { return m_writePos - m_readPos; }
		Shared::int32 FreeSize() { return m_capacity - m_writePos; }

	private:
		Shared::int32 m_capacity = 0;
		Shared::int32 m_bufferSize = 0;
		Shared::int32 m_readPos = 0;
		Shared::int32 m_writePos = 0;
		Shared::List<BYTE> m_recvBuffer;
	};
}

