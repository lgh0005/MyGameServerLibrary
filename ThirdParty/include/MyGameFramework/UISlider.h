#pragma once
#include "UIComponent.h"

namespace MGSL::Framework
{
	class UISlider : public UIComponent
	{
		using Super = UIComponent;

	public:
		virtual ~UISlider() override;

	private:
		UISlider(GameObject* owner);
	};
}

