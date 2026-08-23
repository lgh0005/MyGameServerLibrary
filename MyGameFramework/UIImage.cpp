#include "GameFrameworkPch.h"
#include "UIImage.h"
#include "Texture2D.h"

namespace MGSL::Framework
{
	UIImage::UIImage(GameObject* owner) : Super(owner) {}
	UIImage::~UIImage() = default;

	UIImageUPtr UIImage::Create(GameObject* owner)
	{
		return UIImageUPtr(new UIImage(owner));
	}

	void UIImage::Submit()
	{
		if (!m_texture) return;
	
		UIImageRenderItem renderItem;
		renderItem.texture = m_texture;
		renderItem.instanceData.worldMatrix = GetTransform().GetWorldMatrix();
		renderItem.instanceData.uvRect = m_uvRect;
		renderItem.instanceData.color = m_color;
		renderItem.instanceData.size = GetSize();

		MGSL_RENDER_MGR.Submit(renderItem);
	}

	void UIImage::SetTexture(const Texture2DPtr& texture)
	{
		m_texture = texture;
	}

	void UIImage::SetUVRect(const Shared::vec4& uvRect)
	{
		m_uvRect = uvRect;
	}

	void UIImage::SetColor(const Shared::vec4& color)
	{
		m_color = color;
	}

	const Texture2DPtr& UIImage::GetTexture() const
	{
		return m_texture;
	}

	const Shared::vec4& UIImage::GetUVRect() const
	{
		return m_uvRect;
	}

	const Shared::vec4& UIImage::GetColor() const
	{
		return m_color;
	}
}

