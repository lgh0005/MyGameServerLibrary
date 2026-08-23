#pragma once

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(GLBuffer)
	MGSL_CLASS_PTR(GLVertexArray)

	class GLVertexArray
	{
		MGSL_DISABLE_COPY(GLVertexArray)
		MGSL_DISABLE_MOVE(GLVertexArray)

	public:
		~GLVertexArray();
		static GLVertexArrayUPtr Create();
		void Bind() const;
		static void Unbind();

	/*===============================//
	//   Attribute setting methods   //
	//===============================*/
	public:
		void SetAttrib
		(
			const GLBuffer& vertexBuffer,
			Shared::uint32 attribIndex,
			Shared::uint32 bindingIndex,
			Shared::int32 count,
			GLenum type,
			bool normalized,
			Shared::usize stride,
			Shared::uint64 relativeOffset,
			Shared::uint64 bufferOffset = 0
		) const;

		void SetAttribI
		(
			const GLBuffer& vertexBuffer,
			Shared::uint32 attribIndex,
			Shared::uint32 bindingIndex,
			Shared::int32 count,
			GLenum type,
			Shared::usize stride,
			Shared::uint64 relativeOffset,
			Shared::uint64 bufferOffset = 0
		) const;

		void SetAttribDivisor
		(
			Shared::uint32 bindingIndex,
			Shared::uint32 divisor
		) const;

		void DisableAttrib(Shared::uint32 attribIndex) const;
		void SetIndexBuffer(const GLBuffer& indexBuffer) const;

	/*=====================//
	//   default getters   //
	//=====================*/
	public:
		Shared::uint32 GetVertexArrayID() const;
		bool IsValid() const;

	private:
		GLVertexArray();
		Shared::uint32 m_vertexArrayID = 0;
	};
}

