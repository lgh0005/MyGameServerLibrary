#pragma once

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(GLBuffer)

	class GLBuffer
	{
		MGSL_DISABLE_COPY(GLBuffer)
		MGSL_DISABLE_MOVE(GLBuffer)

	public:
		~GLBuffer();
		static GLBufferUPtr Create
		(
			Shared::usize size,
			const void* data = nullptr,
			GLbitfield storageFlag = 0
		);

		void SetData
		(
			Shared::usize offset,
			Shared::usize size,
			const void* data
		);

		void BindBase
		(
			GLenum target,
			Shared::uint32 bindingPoint
		) const;

	public:
		Shared::uint32 GetBufferID() const;
		Shared::uint64 GetBufferSize() const;
		bool IsValid() const;

	private:
		GLBuffer();

		Shared::uint32 m_bufferID = 0;
		Shared::uint64 m_bufferSize = 0;
		GLbitfield m_storageFlags = 0;
	};
}
