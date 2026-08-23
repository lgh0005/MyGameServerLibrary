#pragma once
#include "Component.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(Texture2D)
	MGSL_CLASS_PTR(SpriteRenderer)

	class SpriteRenderer : public Component
	{
		using Super = Component;

	public:
		virtual ~SpriteRenderer() override;
		static SpriteRendererUPtr Create(GameObject* owner);
		virtual void Submit() override;

	/*============================//
	//   sprite setting methods   //
	//============================*/
	public:
		void SetAtlasTexture(const Texture2DPtr& atlasTexture);
		void SetPixelRect(const Shared::vec4& pixelRect, const Shared::vec2& atlasSize);
		void SetUVRect(const Shared::vec4& uvRect);
		void SetSize(const Shared::vec2& size);
		void SetColor(const Shared::vec4& color);

	/*=====================//
	//   default getters   //
	//=====================*/
	public:
		const Texture2DPtr& GetAtlasTexture() const;
		const Shared::vec4& GetUVRect() const;
		const Shared::vec2& GetSize() const;
		const Shared::vec4& GetColor() const;

	private:
	SpriteRenderer(GameObject* owner);

		Texture2DPtr m_atlasTexture;
		Shared::vec4 m_uvRect{ 0.0f, 0.0f, 1.0f, 1.0f };
		Shared::vec2 m_size{ 1.0f };
		Shared::vec4 m_color{ 1.0f };
	};
}



