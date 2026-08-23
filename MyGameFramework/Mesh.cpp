#include "GameFrameworkPch.h"
#include "Mesh.h"
#include "GLVertexArray.h"
#include "GLBuffer.h"

namespace MGSL::Framework
{
	Mesh::Mesh() = default;
	Mesh::~Mesh() = default;

	void Mesh::Bind() const
	{
		if (!IsValid()) return;
		m_vertexArray->Bind();
	}

	bool Mesh::IsValid() const
	{
		return
			m_vertexArray != nullptr	&&
			m_vertexBuffer != nullptr	&&
			m_indexBuffer != nullptr	&&

			m_vertexArray->IsValid()	&&
			m_vertexBuffer->IsValid()	&&
			m_indexBuffer->IsValid()	&&

			m_vertexCount > 0			&&
			m_indexCount > 0;
	}

	GLVertexArray* Mesh::GetVertexArray() const { return m_vertexArray.get(); }
	GLBuffer* Mesh::GetVertexBuffer()	  const { return m_vertexBuffer.get(); }
	GLBuffer* Mesh::GetIndexBuffer()	  const { return m_indexBuffer.get(); }
	Shared::usize Mesh::GetVertexCount()  const { return m_vertexCount; }
	Shared::usize Mesh::GetIndexCount()   const { return m_indexCount; }
	GLenum Mesh::GetPrimitiveType()		  const { return m_primitiveType; }
	GLenum Mesh::GetIndexType()			  const { return m_indexType; }

	void Mesh::SetMeshMetadata
	(
		Shared::usize vertexCount,
		Shared::usize indexCount,
		GLenum primitiveType,
		GLenum indexType
	)
	{
		m_vertexCount = vertexCount;
		m_indexCount = indexCount;
		m_primitiveType = primitiveType;
		m_indexType = indexType;
	}
}