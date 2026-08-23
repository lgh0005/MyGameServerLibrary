#pragma once
#include "Component.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(UICanvas)

	class UICanvas : public Component
	{
		using Super = Component;

	public:
		virtual ~UICanvas() override;
		static UICanvasUPtr Create(GameObject* owner);

	public:
		Shared::vec2 ScreenToCanvasPoint(const Shared::vec2& screenPoint) const;
		void SetCanvasSize(const Shared::vec2& size);
		const Shared::vec2& GetCanvasSize() const noexcept;

	private:
		UICanvas(GameObject* owner);
		Shared::vec2 m_canvasSize{ 1920.0f, 1080.0f };
	};
}



