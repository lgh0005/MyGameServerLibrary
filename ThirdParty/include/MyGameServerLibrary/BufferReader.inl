#pragma once

namespace MGSL::Net
{
	template<typename T>
	inline bool BufferReader::Peek(T* dest)
	{
		if (!dest)
			return false;

		if (FreeSize() < sizeof(T))
			return false;

		::CopyMemory(dest, &m_readBuffer[m_pos], sizeof(T));
		return true;
	}

	template<typename T>
	inline bool BufferReader::Read(T* dest)
	{
		if (!Peek(dest))
			return false;

		m_pos += sizeof(T);
		return true;
	}

	template<typename T>
	inline BufferReader& BufferReader::operator>>(T& dest)
	{
		dest = *reinterpret_cast<T*>(&m_readBuffer[m_pos]);
		m_pos += sizeof(T);
		return *this;
	}
}