#include "GameFrameworkPch.h"
#include "GLVertexArray.h"
#include "GLBuffer.h"

namespace MGSL::Framework
{
	GLVertexArray::GLVertexArray() = default;
	GLVertexArray::~GLVertexArray()
	{
		if (m_vertexArrayID != 0)
		{
			::glDeleteVertexArrays(1, &m_vertexArrayID);
			m_vertexArrayID = 0;
		}
	}

	GLVertexArrayUPtr GLVertexArray::Create()
	{
		GLVertexArrayUPtr vertexArray(new GLVertexArray());

		::glCreateVertexArrays(1, &vertexArray->m_vertexArrayID);
		if (vertexArray->m_vertexArrayID == 0)
			return nullptr;

		return vertexArray;
	}

	void GLVertexArray::Bind() const
	{
		if (m_vertexArrayID == 0) return;
		::glBindVertexArray(m_vertexArrayID);
	}

	void GLVertexArray::Unbind()
	{
		::glBindVertexArray(0);
	}

	/*===============================//
	//   Attribute setting methods   //
	//===============================*/
	void GLVertexArray::SetAttrib
	(
		const GLBuffer& vertexBuffer,
		Shared::uint32 attribIndex,
		Shared::uint32 bindingIndex,
		Shared::int32 count,
		GLenum type,
		bool normalized,
		Shared::usize stride,
		Shared::uint64 relativeOffset,
		Shared::uint64 bufferOffset
	) const
	{
		::glEnableVertexArrayAttrib(m_vertexArrayID, attribIndex);
		::glVertexArrayAttribFormat(m_vertexArrayID, attribIndex, count, type, normalized, (GLuint)relativeOffset);
		::glVertexArrayAttribBinding(m_vertexArrayID, attribIndex, bindingIndex);
		::glVertexArrayVertexBuffer(m_vertexArrayID, bindingIndex, vertexBuffer.GetBufferID(), (GLintptr)bufferOffset, (GLsizei)stride);
	}

	void GLVertexArray::SetAttribI
	(
		const GLBuffer& vertexBuffer,
		Shared::uint32 attribIndex,
		Shared::uint32 bindingIndex,
		Shared::int32 count,
		GLenum type,
		Shared::usize stride,
		Shared::uint64 relativeOffset,
		Shared::uint64 bufferOffset
	) const
	{
		::glEnableVertexArrayAttrib(m_vertexArrayID, attribIndex);
		::glVertexArrayAttribIFormat(m_vertexArrayID, attribIndex, count, type, (GLuint)relativeOffset);
		::glVertexArrayAttribBinding(m_vertexArrayID, attribIndex, bindingIndex);
		::glVertexArrayVertexBuffer(m_vertexArrayID, bindingIndex, vertexBuffer.GetBufferID(), (GLintptr)bufferOffset, (GLsizei)stride);
	}

	void GLVertexArray::SetAttribDivisor
	(
		Shared::uint32 bindingIndex,
		Shared::uint32 divisor
	) const
	{
		if (m_vertexArrayID == 0) return;
		::glVertexArrayBindingDivisor(m_vertexArrayID, bindingIndex, divisor);
	}

	void GLVertexArray::DisableAttrib(Shared::uint32 attribIndex) const
	{
		if (m_vertexArrayID == 0) return;
		::glDisableVertexArrayAttrib(m_vertexArrayID, attribIndex);
	}

	void GLVertexArray::SetIndexBuffer(const GLBuffer& indexBuffer) const
	{
		::glVertexArrayElementBuffer(m_vertexArrayID, indexBuffer.GetBufferID());
	}

	Shared::uint32 GLVertexArray::GetVertexArrayID() const
	{
		return m_vertexArrayID;
	}

	bool GLVertexArray::IsValid() const
	{
		return m_vertexArrayID != 0;
	}
}