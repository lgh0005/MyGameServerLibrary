#include "GameFrameworkPch.h"
#include "UIComponent.h"
#include "UICanvas.h"

namespace MGSL::Framework
{
	UIComponent::UIComponent(GameObject* owner) : Super(owner) { }
	UIComponent::~UIComponent() = default;

	bool UIComponent::ContainsPoint(const Shared::vec2& point) const
	{
		if (!m_canvas) return false;

		// Screen Space -> Canvas Space
		const Shared::vec2 canvasPoint = m_canvas->ScreenToCanvasPoint(point);

		const Transform& transform = GetTransform();
		const Shared::vec3 worldScale = transform.GetWorldScale();
		MGSL_ASSERT(glm::abs(worldScale.x) > Shared::Epsilon<float>);
		MGSL_ASSERT(glm::abs(worldScale.y) > Shared::Epsilon<float>);
		MGSL_ASSERT(glm::abs(worldScale.z) > Shared::Epsilon<float>);

		const Shared::mat4 inverseWorldMatrix = glm::inverse(transform.GetWorldMatrix());
		const Shared::vec4 localPoint = inverseWorldMatrix * Shared::vec4(canvasPoint.x, canvasPoint.y, 0.0f, 1.0f);
		const Shared::vec2 halfSize = glm::abs(m_size) * 0.5f;

		return
			glm::abs(localPoint.x) <= halfSize.x &&
			glm::abs(localPoint.y) <= halfSize.y;
	}

	void UIComponent::SetSize(const Shared::vec2& size)
	{
		m_size = size;
	}

	const Shared::vec2& UIComponent::GetSize() const noexcept
	{
		return m_size;
	}

	void UIComponent::SetCanvas(UICanvas* canvas)
	{
		m_canvas = canvas;
	}

	UICanvas* UIComponent::GetCanvas() const noexcept
	{
		return m_canvas;
	}
}

