#include "ServerLibPch.h"
#include "RecvBuffer.h"

namespace MGSL::Net
{
	RecvBuffer::RecvBuffer(Shared::int32 bufferSize) : m_bufferSize(bufferSize)
	{
		// 용량은 크기보다 10배 크게
		m_capacity = bufferSize * 10;
		m_recvBuffer.resize(m_capacity);
	}
	RecvBuffer::~RecvBuffer() = default;

	void RecvBuffer::Clean()
	{
		Shared::int32 dataSize = DataSize();
		if (dataSize == 0)
		{
			// 읽기와 쓰기 위치가 동일하면 처음으로 이동
			m_readPos = 0;
			m_writePos = 0;
		}
		else
		{
			// 여유 공간이 버퍼 1개 크기 미만이면, 데이터를 앞으로 옮김.
			if (FreeSize() < m_bufferSize)
			{
				::CopyMemory(m_recvBuffer.data(), &m_recvBuffer[m_readPos], dataSize);
				m_readPos = 0;
				m_writePos = dataSize;
			}
		}
	}

	bool RecvBuffer::OnRead(Shared::int32 numOfBytes)
	{
		// 인자로 들어온 크기만큼 버퍼 상의 데이터를 잘 읽었다면,
		if (numOfBytes > DataSize())
			return false;

		// 읽기 위치를 인자로 들어온 크기만큼 옮김.
		m_readPos += numOfBytes;
		return true;
	}

	bool RecvBuffer::OnWrite(Shared::int32 numOfBytes)
	{
		// 인자로 들어온 크기만큼 버퍼에 기록을 하려는데 남은 공간이 있다면,
		if (numOfBytes > FreeSize())
			return false;

		// 쓰기 위치를 인자로 들어온 크기만큼 옮김.
		m_writePos += numOfBytes;
		return true;
	}
}