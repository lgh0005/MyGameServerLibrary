#pragma once
#include "GPUResource.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(GLSampler)
	MGSL_CLASS_PTR(Font)

	class Font : public GPUResource
	{
		using Super = GPUResource;

	public:
		Font(const fs::path& path, Shared::uint32 pixelSize = 48);
		virtual ~Font() override;

	public:
		virtual bool Load() override;
		virtual bool UploadToGPU() override;

	public:
		const Glyph* GetGlyph(Shared::char32 codepoint) const;
		GLuint GetAtlasTexture() const noexcept;
		const GLSampler* GetSampler() const noexcept;
		Shared::uint32 GetPixelSize() const noexcept;

	private:
		fs::path m_path;
		Shared::uint32 m_pixelSize;
		Shared::Dictionary<Shared::char32, Glyph> m_glyphs;
		Shared::List<Shared::uint8> m_atlasPixels;

		Shared::int32 m_atlasWidth = 1024;
		Shared::int32 m_atlasHeight = 1024;

		GLuint m_atlasTexture = 0;
		GLSamplerUPtr m_fontTextureSampler;
	};
}