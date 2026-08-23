#pragma once

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(GLSampler)

	class GLSampler
	{
		MGSL_DISABLE_COPY(GLSampler)
		MGSL_DISABLE_MOVE(GLSampler)

	public:
		~GLSampler();
		static GLSamplerUPtr Create();

	public:
		void Bind(Shared::uint32 textureUnit) const;
		static void Unbind(Shared::uint32 textureUnit);

	public:
		void SetWrapS(GLenum wrapMode);
		void SetWrapT(GLenum wrapMode);
		void SetWrapR(GLenum wrapMode);
		void SetWrap(GLenum wrapS, GLenum wrapT);

		void SetMinFilter(GLenum filter);
		void SetMagFilter(GLenum filter);
		void SetFilter(GLenum minFilter, GLenum magFilter);

		void SetMaxAnisotropy(float anisotropy);

	public:
		Shared::uint32 GetSamplerID() const;
		bool IsValid() const;

	private:
		GLSampler();
		Shared::uint32 m_samplerID = 0;
	};
}

