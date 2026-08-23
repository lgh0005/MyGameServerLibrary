#pragma once
#include "UIComponent.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(Font)
	MGSL_CLASS_PTR(UIText)

	class UIText : public UIComponent
	{
		using Super = UIComponent;

	public:
		virtual ~UIText() override;
		static UITextUPtr Create(GameObject* owner);
		virtual void Submit() override;

	public:
		void SetText(const Shared::String& text);
		void SetFont(FontPtr font);
		void SetColor(const Shared::vec4& color);

		const Shared::String& GetText() const noexcept;
		const FontPtr& GetFont() const noexcept;
		const Shared::vec4& GetColor() const noexcept;

	private:
		UIText(GameObject* owner);
		void RebuildRenderItem();
		
		Shared::String m_text;
		FontPtr m_font = nullptr;
		Shared::vec4 m_color{ 1.0f };

		UITextRenderItem m_cachedRenderItem;
		Shared::List<Shared::mat4> m_cachedGlyphLocalMatrices;
		bool m_renderItemDirty = true;
	};
}



