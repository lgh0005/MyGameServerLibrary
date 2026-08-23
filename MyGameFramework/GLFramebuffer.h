#pragma once

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(GLFramebuffer)

	class GLFramebuffer
	{
		MGSL_DISABLE_COPY(GLFramebuffer)
		MGSL_DISABLE_MOVE(GLFramebuffer)

	public:
		~GLFramebuffer();
		static GLFramebufferUPtr Create();

		void Bind() const;
		static void Unbind();

	public:
		Shared::uint32 GetFramebufferID() const;
		bool IsAttachmentComplete() const;
		bool IsValid() const;

	/*================================//
	//   default attachment methods   //
	//================================*/
	public:
		void AttachColorTexture
		(
			Shared::uint32 textureID,
			Shared::uint32 attachmentIndex = 0,
			Shared::int32 mipLevel = 0
		) const;

		void AttachDepthTexture
		(
			Shared::uint32 textureID,
			Shared::int32 mipLevel = 0
		) const;

		void AttachStencilTexture
		(
			Shared::uint32 textureID,
			Shared::int32 mipLevel = 0
		) const;

		void AttachDepthStencilTexture
		(
			Shared::uint32 textureID,
			Shared::int32 mipLevel = 0
		) const;

	/*====================================//
	//   framebuffer read/write methods   //
	//====================================*/
	public:
		void SetDrawBuffer(Shared::uint32 attachmentIndex) const;
		void SetReadBuffer(Shared::uint32 attachmentIndex) const;
		void DisableDrawBuffer() const;
		void DisableReadBuffer() const;
		void DisableColorBuffer() const;

	private:
		GLFramebuffer();

		GLuint m_framebufferID = 0;
	};
}

