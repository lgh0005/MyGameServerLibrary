#pragma once

namespace MGSL::Net
{
	template<typename T>
	inline bool BufferWriter::Write(T* src)
	{
		if (!src)
			return false;

		if (FreeSize() < sizeof(T))
			return false;

		::CopyMemory(&m_writeBuffer, src, sizeof(T));
		m_pos += sizeof(T);
		return true;
	}

	template<typename T>
	inline T* BufferWriter::Reserve()
	{
		if (FreeSize() < sizeof(T))
			return nullptr;

		T* ret = reinterpret_cast<T*>(&m_writeBuffer[m_pos]);
		m_pos += sizeof(T);
		return ret;
	}

	template<typename T>
	inline BufferWriter& BufferWriter::operator<<(T&& src)
	{
		using ValueType = Shared::SourceType<T>;
		*reinterpret_cast<ValueType*>(&m_writeBuffer[m_pos]) = std::forward<ValueType>(src);
		m_pos += sizeof(T);
		return *this;
	}
}