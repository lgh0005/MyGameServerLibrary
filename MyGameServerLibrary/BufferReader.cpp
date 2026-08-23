#include "ServerLibPch.h"
#include "BufferReader.h"

namespace MGSL::Net
{
	BufferReader::BufferReader() = default;
	BufferReader::BufferReader
	(
		BYTE* buffer, 
		Shared::uint32 size, 
		Shared::uint32 pos
	) : m_readBuffer(buffer), m_size(size), m_pos(pos) { }
	BufferReader::~BufferReader() = default;
}