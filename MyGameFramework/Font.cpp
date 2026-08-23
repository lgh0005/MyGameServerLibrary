#include "GameFrameworkPch.h"
#include "Font.h"
#include "GLSampler.h"

namespace MGSL::Framework
{
	Font::Font(const fs::path& path, Shared::uint32 pixelSize) : Super(), m_path(path), m_pixelSize(pixelSize) { }
	Font::~Font()
	{
		if (m_atlasTexture != 0)
		{
			::glDeleteTextures(1, &m_atlasTexture);
			m_atlasTexture = 0;
		}
	}

	bool Font::Load()
	{
		FT_Library library = nullptr;
		if (FT_Init_FreeType(&library)) return false;

		FT_Face face = nullptr;
		if (FT_New_Face(library, m_path.string().c_str(), 0, &face))
		{
			FT_Done_FreeType(library);
			return false;
		}

		if (FT_Set_Pixel_Sizes(face, 0, m_pixelSize))
		{
			FT_Done_Face(face);
			FT_Done_FreeType(library);
			return false;
		}

		m_glyphs.clear();
		m_atlasPixels.assign(m_atlasWidth * m_atlasHeight, 0);

		constexpr Shared::int32 padding = 1;
		Shared::int32 penX = padding;
		Shared::int32 penY = padding;
		Shared::int32 rowHeight = 0;

		for (Shared::char32 codepoint = 32; codepoint < 127; ++codepoint)
		{
			if (FT_Load_Char(face, static_cast<FT_ULong>(codepoint), FT_LOAD_RENDER))
				continue;

			const FT_GlyphSlot glyphSlot = face->glyph;
			const FT_Bitmap& bitmap = glyphSlot->bitmap;
			const Shared::int32 glyphWidth = static_cast<Shared::int32>(bitmap.width);
			const Shared::int32 glyphHeight = static_cast<Shared::int32>(bitmap.rows);

			// 현재 행에 들어가지 않으면 다음 행으로 이동
			if (penX + glyphWidth + padding > m_atlasWidth)
			{
				penX = padding;
				penY += rowHeight + padding;
				rowHeight = 0;
			}

			// Atlas 자체가 부족한 경우
			if (penY + glyphHeight + padding > m_atlasHeight)
			{
				FT_Done_Face(face);
				FT_Done_FreeType(library);
				return false;
			}

			// Glyph Bitmap → Atlas
			for (Shared::int32 y = 0; y < glyphHeight; ++y)
			{
				for (Shared::int32 x = 0; x < glyphWidth; ++x)
				{
					const Shared::int32 atlasX = penX + x;
					const Shared::int32 atlasY = penY + y;

					m_atlasPixels[atlasY * m_atlasWidth + atlasX] = bitmap.buffer[y * bitmap.pitch + x];
				}
			}

			Glyph glyph;
			glyph.size = { glyphWidth, glyphHeight };
			glyph.bearing = { glyphSlot->bitmap_left, glyphSlot->bitmap_top };
			glyph.advance = static_cast<Shared::uint32>(glyphSlot->advance.x);
			glyph.uvMin = { (float)penX / (float)m_atlasWidth, (float)penY / (float)m_atlasHeight };
			glyph.uvMax = { (float)(penX + glyphWidth) / (float)m_atlasWidth, (float)(penY + glyphHeight) / (float)m_atlasHeight };
			m_glyphs.emplace(codepoint, glyph);

			penX += glyphWidth + padding;
			rowHeight = std::max(rowHeight, glyphHeight);
		}

		FT_Done_Face(face);
		FT_Done_FreeType(library);

		return true;
	}

	bool Font::UploadToGPU()
	{
		if (m_atlasPixels.empty()) return false;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_atlasTexture);
		if (m_atlasTexture == 0) return false;

		glTextureStorage2D(m_atlasTexture, 1, GL_R8, m_atlasWidth, m_atlasHeight);
		::glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTextureSubImage2D(m_atlasTexture, 0, 0, 0, m_atlasWidth, m_atlasHeight, GL_RED, GL_UNSIGNED_BYTE, m_atlasPixels.data());
		::glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		m_fontTextureSampler = GLSampler::Create();
		if (!m_fontTextureSampler)
		{
			::glDeleteTextures(1, &m_atlasTexture);
			m_atlasTexture = 0;
			return false;
		}

		return true;
	}

	const Glyph* Font::GetGlyph(Shared::char32 codepoint) const
	{
		const auto it = m_glyphs.find(codepoint);
		if (it == m_glyphs.end()) return nullptr;
		return &it->second;
	}

	GLuint Font::GetAtlasTexture() const noexcept
	{
		return m_atlasTexture;
	}

	const GLSampler* Font::GetSampler() const noexcept
	{
		return m_fontTextureSampler.get();
	}

	Shared::uint32 Font::GetPixelSize() const noexcept
	{
		return m_pixelSize;
	}
}