#pragma once

namespace MGSL::Net
{
	class BufferReader
	{
	public:
		BufferReader();
		BufferReader
		(
			BYTE* buffer, 
			Shared::uint32 size, 
			Shared::uint32 pos = 0
		);
		~BufferReader();

	public:
		BYTE*		   Buffer()   { return m_readBuffer; }
		Shared::uint32 Size()     { return m_size; }
		Shared::uint32 ReadSize() { return m_pos; }
		Shared::uint32 FreeSize() { return m_size - m_pos; }

	public:
		template<typename T> bool Peek(T* dest);
		template<typename T> bool Read(T* dest);
		template<typename T> BufferReader& operator>>(T& dest);

	private:
		BYTE* m_readBuffer = nullptr;
		Shared::uint32 m_size = 0;
		Shared::uint32 m_pos = 0;
	};
}

#include "BufferReader.inl"
