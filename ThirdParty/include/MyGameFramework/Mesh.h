#pragma once

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(GLVertexArray)
	MGSL_CLASS_PTR(GLBuffer)

	class Mesh
	{
		MGSL_DISABLE_COPY(Mesh)
		MGSL_DISABLE_MOVE(Mesh)

	public:
		virtual ~Mesh();
		void Bind() const;
		bool IsValid() const;

	public:
		GLVertexArray* GetVertexArray() const;
		GLBuffer* GetVertexBuffer() const;
		GLBuffer* GetIndexBuffer() const;
		Shared::usize GetVertexCount() const;
		Shared::usize GetIndexCount() const;
		GLenum GetPrimitiveType() const;
		GLenum GetIndexType() const;

	protected:
		template<typename T>
		bool CreateOpenGLObjects
		(
			const Shared::List<T>& vertices,
			const Shared::List<Shared::uint32>& indices
		);

		void SetMeshMetadata
		(
			Shared::usize vertexCount,
			Shared::usize indexCount,
			GLenum primitiveType = GL_TRIANGLES,
			GLenum indexType = GL_UNSIGNED_INT
		);

	protected:
		Mesh();

		GLVertexArrayUPtr m_vertexArray;
		GLBufferUPtr m_vertexBuffer;
		GLBufferUPtr m_indexBuffer;

		Shared::usize m_vertexCount = 0;
		Shared::usize m_indexCount = 0;

		GLenum m_primitiveType = GL_TRIANGLES;
		GLenum m_indexType = GL_UNSIGNED_INT;
	};
}

#include "Mesh.inl"