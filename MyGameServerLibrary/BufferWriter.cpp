#include "ServerLibPch.h"
#include "BufferWriter.h"

namespace MGSL::Net
{
	BufferWriter::BufferWriter() = default;
	BufferWriter::BufferWriter
	(
		BYTE* buffer, 
		Shared::uint32 size,
		Shared::uint32 pos
	) : m_writeBuffer(buffer), m_size(size), m_pos(pos) { }
	BufferWriter::~BufferWriter() = default;
}