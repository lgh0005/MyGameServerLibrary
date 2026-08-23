#include "GameFrameworkPch.h"
#include "GLBuffer.h"

namespace MGSL::Framework
{
	GLBuffer::GLBuffer() = default;
	GLBuffer::~GLBuffer()
	{
		if (!IsValid()) return;

		::glDeleteBuffers(1, &m_bufferID);
		m_bufferID = 0;
		m_bufferSize = 0;
		m_storageFlags = 0;
	}

	GLBufferUPtr GLBuffer::Create
	(
		Shared::usize size,
		const void* data,
		GLbitfield storageFlags
	)
	{
		if (size == 0) return nullptr;

		GLBufferUPtr buffer(new GLBuffer());

		::glCreateBuffers(1, &buffer->m_bufferID);
		if (!buffer->IsValid()) return nullptr;

		::glNamedBufferStorage
		(
			buffer->m_bufferID,
			static_cast<GLsizeiptr>(size),
			data,
			storageFlags
		);

		buffer->m_bufferSize = size;
		buffer->m_storageFlags = storageFlags;

		return buffer;
	}

	void GLBuffer::SetData
	(
		Shared::usize offset,
		Shared::usize size,
		const void* data
	)
	{
		::glNamedBufferSubData
		(
			m_bufferID,
			static_cast<GLintptr>(offset),
			static_cast<GLsizeiptr>(size),
			data
		);
	}

	void GLBuffer::BindBase
	(
		GLenum target,
		Shared::uint32 bindingPoint
	) const
	{
		if (!IsValid()) return;
		::glBindBufferBase
		(
			target,
			static_cast<GLuint>(bindingPoint),
			m_bufferID
		);
	}

	Shared::uint32 GLBuffer::GetBufferID() const
	{
		return m_bufferID;
	}

	Shared::usize GLBuffer::GetBufferSize() const
	{
		return m_bufferSize;
	}

	bool GLBuffer::IsValid() const
	{
		return m_bufferID != 0;
	}
}