#include "GameFrameworkPch.h"
#include "GLFramebuffer.h"

namespace MGSL::Framework
{
	GLFramebuffer::GLFramebuffer() = default;
	GLFramebuffer::~GLFramebuffer()
	{
		if (!IsValid()) return;
		::glDeleteFramebuffers(1, &m_framebufferID);
		m_framebufferID = 0;
	}

	GLFramebufferUPtr GLFramebuffer::Create()
	{
		GLFramebufferUPtr framebuffer(new GLFramebuffer());

		::glCreateFramebuffers(1, &framebuffer->m_framebufferID);
		if (!framebuffer->IsValid())
			return nullptr;

		return framebuffer;
	}

	void GLFramebuffer::Bind() const
	{
		::glBindFramebuffer(GL_FRAMEBUFFER, m_framebufferID);
	}

	void GLFramebuffer::Unbind()
	{
		::glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	Shared::uint32 GLFramebuffer::GetFramebufferID() const
	{
		return m_framebufferID;
	}

	bool GLFramebuffer::IsAttachmentComplete() const
	{
		const GLenum status = ::glCheckNamedFramebufferStatus(m_framebufferID, GL_FRAMEBUFFER);
		if (status == GL_FRAMEBUFFER_COMPLETE)
			return true;

		MGSL_LOG_ERROR
		(
			"Framebuffer attachment validation failed. Status code: {}",
			static_cast<Shared::uint32>(status)
		);

		return false;
	}

	bool GLFramebuffer::IsValid() const
	{
		return m_framebufferID != 0;
	}

	/*================================//
	//   default attachment methods   //
	//================================*/
	void GLFramebuffer::AttachColorTexture
	(
		Shared::uint32 textureID,
		Shared::uint32 attachmentIndex,
		Shared::int32 mipLevel
	) const
	{
		::glNamedFramebufferTexture
		(
			m_framebufferID,
			GL_COLOR_ATTACHMENT0 + attachmentIndex,
			textureID,
			mipLevel
		);
	}

	void GLFramebuffer::AttachDepthTexture
	(
		Shared::uint32 textureID,
		Shared::int32 mipLevel
	) const
	{
		::glNamedFramebufferTexture
		(
			m_framebufferID,
			GL_DEPTH_ATTACHMENT,
			textureID,
			mipLevel
		);
	}

	void GLFramebuffer::AttachStencilTexture
	(
		Shared::uint32 textureID,
		Shared::int32 mipLevel
	) const
	{
		::glNamedFramebufferTexture
		(
			m_framebufferID,
			GL_STENCIL_ATTACHMENT,
			textureID,
			mipLevel
		);
	}

	void GLFramebuffer::AttachDepthStencilTexture
	(
		Shared::uint32 textureID,
		Shared::int32 mipLevel
	) const
	{
		::glNamedFramebufferTexture
		(
			m_framebufferID,
			GL_DEPTH_STENCIL_ATTACHMENT,
			textureID,
			mipLevel
		);
	}

	/*====================================//
	//   framebuffer read/write methods   //
	//====================================*/
	void GLFramebuffer::SetDrawBuffer(Shared::uint32 attachmentIndex) const
	{
		::glNamedFramebufferDrawBuffer(m_framebufferID, GL_COLOR_ATTACHMENT0 + attachmentIndex);
	}

	void GLFramebuffer::SetReadBuffer(Shared::uint32 attachmentIndex) const
	{
		::glNamedFramebufferReadBuffer(m_framebufferID, GL_COLOR_ATTACHMENT0 + attachmentIndex);
	}

	void GLFramebuffer::DisableDrawBuffer() const
	{
		::glNamedFramebufferDrawBuffer(m_framebufferID, GL_NONE);
	}

	void GLFramebuffer::DisableReadBuffer() const
	{
		::glNamedFramebufferReadBuffer(m_framebufferID, GL_NONE);
	}

	void GLFramebuffer::DisableColorBuffer() const
	{
		DisableDrawBuffer();
		DisableReadBuffer();
	}
}