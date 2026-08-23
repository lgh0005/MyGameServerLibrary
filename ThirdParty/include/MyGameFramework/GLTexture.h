#pragma once
#include <ktx/ktx.h>

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(GLTexture)

	class GLTexture
	{
		MGSL_DISABLE_COPY(GLTexture)
		MGSL_DISABLE_MOVE(GLTexture)

	public:
		~GLTexture();
		static GLTextureUPtr Create(ktxTexture* texture);
		void Bind(Shared::uint32 textureUnit) const;

	public:
		Shared::uint32 GetTextureID() const;
		GLenum GetTarget() const;
		bool IsValid() const;

	private:
		GLTexture();

		Shared::uint32 m_textureID = 0;
		GLenum m_target = 0;
	};
}

