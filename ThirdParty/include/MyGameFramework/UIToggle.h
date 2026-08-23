#pragma once
#include "UIComponent.h"

namespace MGSL::Framework
{
	class UIToggle : public UIComponent
	{
		using Super = UIComponent;

	public:
		virtual ~UIToggle() override;

	private:
		UIToggle(GameObject* owner);
	};
}

