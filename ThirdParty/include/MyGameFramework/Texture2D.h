#pragma once
#include "GPUResource.h"
#include <ktx/ktx.h>

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(GLTexture)
	MGSL_CLASS_PTR(GLSampler)

	class Texture2D : public GPUResource
	{
		using Super = GPUResource;

	public:
		explicit Texture2D(fs::path path);
		virtual ~Texture2D() override;

	/*================================//
	//   default texture-2d methods   //
	//================================*/
	public:
		void Bind(Shared::uint32 textureUnit) const;
		bool IsValid() const;

	/*==========================//
	//   Interface implements   //
	//==========================*/
	public:
		virtual bool Load() override;
		virtual bool UploadToGPU() override;

	private:
		fs::path m_path;
		ktxTexture* m_ktxTexture = nullptr;

		GLTextureUPtr m_texture;
		GLSamplerUPtr m_sampler;
	};
}

