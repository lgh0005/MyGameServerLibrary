#include "GameFrameworkPch.h"
#include "UICanvas.h"

namespace MGSL::Framework
{
	UICanvas::UICanvas(GameObject* owner) : Super(owner) {}
	UICanvas::~UICanvas() = default;

	UICanvasUPtr UICanvas::Create(GameObject* owner)
	{
		return UICanvasUPtr(new UICanvas(owner));
	}

	Shared::vec2 UICanvas::ScreenToCanvasPoint(const Shared::vec2& screenPoint) const
	{
		return { screenPoint.x - m_canvasSize.x * 0.5f, m_canvasSize.y * 0.5f - screenPoint.y };
	}

	void UICanvas::SetCanvasSize(const Shared::vec2& size)
	{
		MGSL_ASSERT(size.x > Shared::Epsilon<float>);
		MGSL_ASSERT(size.y > Shared::Epsilon<float>);
		m_canvasSize = size;
	}

	const Shared::vec2& UICanvas::GetCanvasSize() const noexcept
	{
		return m_canvasSize;
	}
}

