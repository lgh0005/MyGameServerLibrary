#pragma once
#include "MyGameFramework/Component.h"

namespace MGSL::Sandbox2D
{
	MGSL_CLASS_PTR(AttackEffect)

	class AttackEffect : public Framework::Component
	{
		MGSL_DISABLE_COPY(AttackEffect)
		MGSL_DISABLE_MOVE(AttackEffect)
		using Super = Framework::Component;

	public:
		virtual ~AttackEffect() override;
		static AttackEffectUPtr Create(Framework::GameObject* owner);

	public:
		virtual void Update(float deltaTime) override;

	private:
		explicit AttackEffect(Framework::GameObject* owner);
	};
}