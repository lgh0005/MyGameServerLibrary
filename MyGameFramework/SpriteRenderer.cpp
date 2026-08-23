#include "GameFrameworkPch.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "RenderItem.h"
#include "RenderManager.h"
#include "Transform.h"

namespace MGSL::Framework
{
	SpriteRenderer::SpriteRenderer(GameObject* owner) : Super(owner) { }
	SpriteRenderer::~SpriteRenderer() = default;

	SpriteRendererUPtr SpriteRenderer::Create(GameObject* owner)
	{
		if (owner == nullptr) return nullptr;
		return SpriteRendererUPtr(new SpriteRenderer(owner));
	}

	void SpriteRenderer::Submit()
	{
		if (!m_atlasTexture) return;

		GameObject* owner = GetOwner();
		if (!owner) return;

		SpriteRenderItem renderItem;
		renderItem.atlasTexture = m_atlasTexture;
		renderItem.instanceData.worldMatrix = owner->GetTransform().GetWorldMatrix();
		renderItem.instanceData.uvRect = m_uvRect;
		renderItem.instanceData.size = m_size;
		renderItem.instanceData.color = m_color;

		MGSL_RENDER_MGR.Submit(renderItem);
	}

	/*============================//
	//   sprite setting methods   //
	//============================*/
	void SpriteRenderer::SetAtlasTexture(const Texture2DPtr& atlasTexture) { m_atlasTexture = atlasTexture; }
	void SpriteRenderer::SetUVRect(const Shared::vec4& uvRect) { m_uvRect = uvRect; }
	void SpriteRenderer::SetSize(const Shared::vec2& size) { m_size = size; }
	void SpriteRenderer::SetColor(const Shared::vec4& color) { m_color = color; }

	/*=====================//
	//   default getters   //
	//=====================*/
	const Texture2DPtr& SpriteRenderer::GetAtlasTexture() const { return m_atlasTexture; }
	const Shared::vec4& SpriteRenderer::GetUVRect() const { return m_uvRect; }
	const Shared::vec2& SpriteRenderer::GetSize() const { return m_size; }
	const Shared::vec4& SpriteRenderer::GetColor() const { return m_color; }
}


