#pragma once
#include "UIComponent.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(UIButton)

	class UIButton : public UIComponent
	{
		using Super = UIComponent;
		using Callback = Shared::Action<>;

	public:
		virtual ~UIButton() override;
		static UIButtonUPtr Create(GameObject* owner);

	public:
		virtual void Update([[maybe_unused]] float deltaTime) override;

	public:
		void SetInteractable(bool interactable);
		void SetOnPointerEnter(Callback callback);
		void SetOnPointerExit(Callback callback);
		void SetOnClick(Callback callback);

	public:
		bool IsInteractable() const noexcept;
		EUIButtonState GetState() const noexcept;

	private:
		UIButton(GameObject* owner);

		void SetState(EUIButtonState state);
		void HandleButtonStates();
		void InvokeCallback(const Callback& callback);

		bool m_interactable = true;
		EUIButtonState m_state = EUIButtonState::NORMAL;

		Callback m_onPointerEnter;
		Callback m_onPointerExit;
		Callback m_onClick;
	};
}

