#include "GameFrameworkPch.h"
#include "GLSampler.h"

namespace MGSL::Framework
{
	GLSampler::GLSampler() = default;
	GLSampler::~GLSampler()
	{
		if (m_samplerID != 0)
		{
			::glDeleteSamplers(1, &m_samplerID);
			m_samplerID = 0;
		}
	}

	GLSamplerUPtr GLSampler::Create()
	{
		GLSamplerUPtr sampler(new GLSampler());
		::glCreateSamplers(1, &sampler->m_samplerID);
		if (!sampler->IsValid()) return nullptr;

		// 기본 샘플링 설정
		sampler->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		sampler->SetFilter(GL_LINEAR, GL_LINEAR);

		return sampler;
	}

	void GLSampler::Bind(Shared::uint32 textureUnit) const
	{
		if (!IsValid()) return;
		::glBindSampler(textureUnit, m_samplerID);
	}

	void GLSampler::Unbind(Shared::uint32 textureUnit)
	{
		::glBindSampler(textureUnit, 0);
	}

	void GLSampler::SetWrapS(GLenum wrapMode)
	{
		if (!IsValid()) return;
		::glSamplerParameteri(m_samplerID, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrapMode));
	}

	void GLSampler::SetWrapT(GLenum wrapMode)
	{
		if (!IsValid()) return;
		::glSamplerParameteri(m_samplerID, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrapMode));
	}

	void GLSampler::SetWrapR(GLenum wrapMode)
	{
		if (!IsValid()) return;
		::glSamplerParameteri(m_samplerID, GL_TEXTURE_WRAP_R, static_cast<GLint>(wrapMode));
	}

	void GLSampler::SetWrap(GLenum wrapS, GLenum wrapT)
	{
		SetWrapS(wrapS);
		SetWrapT(wrapT);
	}

	void GLSampler::SetMinFilter(GLenum filter)
	{
		if (!IsValid()) return;
		::glSamplerParameteri(m_samplerID, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(filter));
	}

	void GLSampler::SetMagFilter(GLenum filter)
	{
		if (!IsValid()) return;
		::glSamplerParameteri(m_samplerID, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(filter));
	}

	void GLSampler::SetFilter(GLenum minFilter, GLenum magFilter)
	{
		SetMinFilter(minFilter);
		SetMagFilter(magFilter);
	}

	void GLSampler::SetMaxAnisotropy(float anisotropy)
	{
		if (!IsValid()) return;
		if (!GLEW_EXT_texture_filter_anisotropic) return;

		float maxAnisotropy = 1.0f;
		::glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);

		const float clampedAnisotropy = glm::clamp(anisotropy, 1.0f, maxAnisotropy);
		::glSamplerParameterf(m_samplerID, GL_TEXTURE_MAX_ANISOTROPY_EXT, clampedAnisotropy);
	}

	Shared::uint32 GLSampler::GetSamplerID() const { return m_samplerID; }
	bool GLSampler::IsValid() const { return m_samplerID != 0; }
}
