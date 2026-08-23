#pragma once
#include "UIComponent.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(Texture2D)
	MGSL_CLASS_PTR(UIImage)

	class UIImage : public UIComponent
	{
		using Super = UIComponent;

	public:
		virtual ~UIImage() override;
		static UIImageUPtr Create(GameObject* owner);
		virtual void Submit() override;

	public:
		void SetTexture(const Texture2DPtr& texture);
		void SetUVRect(const Shared::vec4& uvRect);
		void SetColor(const Shared::vec4& color);

	public:
		const Texture2DPtr& GetTexture() const;
		const Shared::vec4& GetUVRect() const;
		const Shared::vec4& GetColor() const;

	private:
		UIImage(GameObject* owner);

		Texture2DPtr m_texture;
		Shared::vec4 m_uvRect{ 0.0f, 0.0f, 1.0f, 1.0f };
		Shared::vec4 m_color{ 1.0f };
	};
}



