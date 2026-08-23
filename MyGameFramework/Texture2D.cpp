#include "GameFrameworkPch.h"
#include "Texture2D.h"
#include "GLTexture.h"
#include "GLSampler.h"

namespace MGSL::Framework
{
	Texture2D::Texture2D(fs::path path) : m_path(std::move(path)) { }
	Texture2D::~Texture2D()
	{
		if (m_ktxTexture)
		{
			ktxTexture_Destroy(m_ktxTexture);
			m_ktxTexture = nullptr;
		}
	}

	/*================================//
	//   default texture-2d methods   //
	//================================*/
	void Texture2D::Bind(Shared::uint32 textureUnit) const
	{
		if (!m_texture || !m_sampler) return;
		m_texture->Bind(textureUnit);
		m_sampler->Bind(textureUnit);
	}

	bool Texture2D::IsValid() const
	{
		return
			m_texture &&
			m_texture->IsValid() &&
			m_sampler &&
			m_sampler->IsValid();
	}

	/*==========================//
	//   Interface implements   //
	//==========================*/
	bool Texture2D::Load()
	{
		const Shared::String path = m_path.string();
		const KTX_error_code result =
			::ktxTexture_CreateFromNamedFile
			(
				path.c_str(),
				KTX_TEXTURE_CREATE_LOAD_IMAGE_DATA_BIT,
				&m_ktxTexture
			);
		if (result != KTX_SUCCESS)
		{
			m_ktxTexture = nullptr;
			return false;
		}

		const bool isTexture2D =
			m_ktxTexture->numDimensions == 2 &&
			!m_ktxTexture->isCubemap &&
			!m_ktxTexture->isArray;
		
		if (!isTexture2D)
		{
			ktxTexture_Destroy(m_ktxTexture);
			m_ktxTexture = nullptr;
			return false;
		}

		// 텍스처 트랜스코딩
		if (m_ktxTexture->classId == ktxTexture2_c)
		{
			ktxTexture2* texture2 = reinterpret_cast<ktxTexture2*>(m_ktxTexture);

			if (::ktxTexture2_NeedsTranscoding(texture2))
			{
				const KTX_error_code transcodeResult = ::ktxTexture2_TranscodeBasis(texture2, KTX_TTF_RGBA32, 0);
				if (transcodeResult != KTX_SUCCESS)
				{
					ktxTexture_Destroy(m_ktxTexture);
					m_ktxTexture = nullptr;
					return false;
				}
			}
		}

		return true;
	}

	bool Texture2D::UploadToGPU()
	{
		if (!m_ktxTexture) return false;

		// 텍스처 생성
		GLTextureUPtr texture = GLTexture::Create(m_ktxTexture);
		if (!texture) return false;
		if (texture->GetTarget() != GL_TEXTURE_2D) return false;

		// 샘플러 생성
		GLSamplerUPtr sampler = GLSampler::Create();
		if (!sampler) return false;

		bool hasMipmap = m_ktxTexture->numLevels > 1;
		sampler->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
		sampler->SetFilter(hasMipmap ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR, GL_LINEAR);

		// CPU-side KTX 데이터 정리
		ktxTexture_Destroy(m_ktxTexture);
		m_ktxTexture = nullptr;

		m_texture = std::move(texture);
		m_sampler = std::move(sampler);

		return true;
	}
}
