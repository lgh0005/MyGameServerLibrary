#pragma once
#include "Component.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(UICanvas)

	class UIComponent : public Component
	{
		using Super = Component;

	public:
		virtual ~UIComponent() override;

	public:
		bool ContainsPoint(const Shared::vec2& point) const;
		void SetSize(const Shared::vec2& size);
		const Shared::vec2& GetSize() const noexcept;

	public:
		void SetCanvas(UICanvas* canvas);
		UICanvas* GetCanvas() const noexcept;

	protected:
		explicit UIComponent(GameObject* owner);

	private:
		Shared::vec2 m_size { 100.0f, 100.0f };
		UICanvas* m_canvas = nullptr;
	};
}


