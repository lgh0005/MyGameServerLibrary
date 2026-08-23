#include "GameFrameworkPch.h"
#include "GLTexture.h"

namespace MGSL::Framework
{
	GLTexture::GLTexture() = default;
	GLTexture::~GLTexture()
	{
		if (m_textureID != 0)
		{
			::glDeleteTextures(1, &m_textureID);
			m_textureID = 0;
			m_target = 0;
		}
	}

	GLTextureUPtr GLTexture::Create(ktxTexture* texture)
	{
		if (!texture) return nullptr;

		GLTextureUPtr glTexture(new GLTexture());

		GLenum glError = GL_NO_ERROR;
		const KTX_error_code result =
			::ktxTexture_GLUpload
			(
				texture,
				&glTexture->m_textureID,
				&glTexture->m_target,
				&glError
			);
		if (result != KTX_SUCCESS)
			return nullptr;

		if (glTexture->m_textureID == 0) 
			return nullptr;

		return glTexture;
	}

	void GLTexture::Bind(Shared::uint32 textureUnit) const
	{
		if (m_textureID == 0) return;
		::glBindTextureUnit(textureUnit, m_textureID);
	}

	Shared::uint32 GLTexture::GetTextureID() const
	{
		return m_textureID;
	}

	GLenum GLTexture::GetTarget() const
	{
		return m_target;
	}

	bool GLTexture::IsValid() const
	{
		return m_textureID != 0 && m_target != 0;
	}
}
