#pragma once

namespace MGSL::Net
{
	class BufferWriter
	{
	public:
		BufferWriter();
		BufferWriter
		(
			BYTE* buffer, 
			Shared::uint32 size, 
			Shared::uint32 pos = 0
		);
		~BufferWriter();

	public:
		BYTE*		   Buffer()    { return m_writeBuffer; }
		Shared::uint32 Size()      { return m_size; }
		Shared::uint32 WriteSize() { return m_pos; }
		Shared::uint32 FreeSize()  { return m_size - m_pos; }

	public:
		template<typename T> bool Write(T* src);
		template<typename T> T* Reserve();
		template<typename T> BufferWriter& operator<<(T&& src);

	private:
		BYTE* m_writeBuffer = nullptr;
		Shared::uint32 m_size = 0;
		Shared::uint32 m_pos = 0;
	};
}

#include "BufferWriter.inl"