#include "GameFrameworkPch.h"
#include "UIText.h"
#include "Font.h"
#include "Glyph.h"
#include "RenderManager.h"

namespace MGSL::Framework
{
	UIText::UIText(GameObject* owner) : Super(owner) { }
	UIText::~UIText() = default;

	UITextUPtr UIText::Create(GameObject* owner)
	{
		return UITextUPtr(new UIText(owner));
	}

	void UIText::Submit()
	{
		if (!m_font || m_text.empty()) return;
		if (m_renderItemDirty) RebuildRenderItem();
		if (m_cachedRenderItem.instances.empty()) return;

		const Shared::mat4& textWorldMatrix = GetTransform().GetWorldMatrix();
		for (Shared::usize i = 0; i < m_cachedRenderItem.instances.size(); i++)
			m_cachedRenderItem.instances[i].worldMatrix = textWorldMatrix * m_cachedGlyphLocalMatrices[i];

		MGSL_RENDER_MGR.Submit(m_cachedRenderItem);
	}

	void UIText::RebuildRenderItem()
	{
		m_cachedRenderItem.font = m_font;
		m_cachedRenderItem.instances.clear();
		m_cachedGlyphLocalMatrices.clear();
		if (!m_font || m_text.empty())
		{
			m_renderItemDirty = false;
			return;
		}

		m_cachedRenderItem.instances.reserve(m_text.size());
		m_cachedGlyphLocalMatrices.reserve(m_text.size());

		float penX = 0.0f;
		for (const Shared::uint8 character : m_text)
		{
			const Shared::char32 codepoint = static_cast<Shared::char32>(character);
			const Glyph* glyph = m_font->GetGlyph(codepoint);
			if (!glyph) continue;

			const float glyphWidth = static_cast<float>(glyph->size.x);
			const float glyphHeight = static_cast<float>(glyph->size.y);
			const float glyphCenterX = penX + static_cast<float>(glyph->bearing.x) + glyphWidth * 0.5f;
			const float glyphCenterY = static_cast<float>(glyph->bearing.y) - glyphHeight * 0.5f;
			const Shared::mat4 glyphLocalMatrix = glm::translate(Shared::mat4(1.0f), Shared::vec3(glyphCenterX, glyphCenterY, 0.0f));
		
			UIInstanceData instanceData;
			instanceData.worldMatrix = Shared::mat4(1.0f);
			instanceData.uvRect = { glyph->uvMin.x, glyph->uvMin.y, glyph->uvMax.x - glyph->uvMin.x, glyph->uvMax.y - glyph->uvMin.y };
			instanceData.color = m_color;
			instanceData.size = { glyphWidth, glyphHeight };

			m_cachedRenderItem.instances.push_back(instanceData);
			m_cachedGlyphLocalMatrices.push_back(glyphLocalMatrix);

			penX += static_cast<float>(glyph->advance >> 6);
		}

		m_renderItemDirty = false;
	}
	
	void UIText::SetText(const Shared::String& text)
	{
		if (m_text == text) return;
		m_text = text;
		m_renderItemDirty = true;
	}

	void UIText::SetFont(FontPtr font)
	{
		if (m_font == font) return;
		m_font = std::move(font);
		m_renderItemDirty = true;
	}

	void UIText::SetColor(const Shared::vec4& color)
	{
		if (m_color == color) return;
		m_color = color;
		m_renderItemDirty = true;
	}

	const Shared::String& UIText::GetText() const noexcept
	{
		return m_text;
	}

	const FontPtr& UIText::GetFont() const noexcept
	{
		return m_font;
	}

	const Shared::vec4& UIText::GetColor() const noexcept
	{
		return m_color;
	}
}

