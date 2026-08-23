#pragma once
#include "GLBuffer.h"

namespace MGSL::Framework
{
	template<typename T>
	inline bool Mesh::CreateOpenGLObjects
	(
		const Shared::List<T>& vertices,
		const Shared::List<Shared::uint32>& indices
	)
	{
		if (vertices.empty()) return false;
		if (indices.empty()) return false;

		m_vertexArray = GLVertexArray::Create();
		if (m_vertexArray == nullptr)
			return false;

		m_vertexBuffer = GLBuffer::Create(sizeof(T) * vertices.size(), vertices.data(), 0);
		if (m_vertexBuffer == nullptr)
			return false;

		m_indexBuffer = GLBuffer::Create(sizeof(Shared::uint32) * indices.size(), indices.data(), 0);
		if (m_indexBuffer == nullptr)
			return false;

		return true;
	}
}