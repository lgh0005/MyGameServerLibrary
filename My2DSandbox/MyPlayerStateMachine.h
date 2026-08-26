#pragma once
#include "MyGameFramework/MonoBehaviour.h"

namespace MGSL::Framework { MGSL_CLASS_PTR(FlipbookPlayer) }

namespace MGSL::Sandbox2D
{
	MGSL_CLASS_PTR(MyPlayerNetworkState)
	MGSL_CLASS_PTR(MyPlayerStateMachine)

	class MyPlayerStateMachine : public Framework::MonoBehaviour
	{
		MGSL_DISABLE_COPY(MyPlayerStateMachine)
		MGSL_DISABLE_MOVE(MyPlayerStateMachine)
		using Super = Framework::MonoBehaviour;

	public:
		virtual ~MyPlayerStateMachine() override;
		static MyPlayerStateMachineUPtr Create(Framework::GameObject* owner);
		virtual void Awake() override;
		virtual void Update(float deltaTime) override;

	private:
		explicit MyPlayerStateMachine(Framework::GameObject* owner);

		void ApplyState();
		void ApplyFacing();
		void ApplyWeapon();
		void ApplyColor();

	private:
		void UpdateClimbAnimation();
		void UpdateInvincibleEffect(float deltaTime);

	private:
		MyPlayerNetworkState* m_playerNetworkState = nullptr;
		Framework::FlipbookPlayer* m_flipbookPlayer = nullptr;

		Protobuf::OBJECT_STATE_TYPE m_prevState{};
		Protobuf::FACING_TYPE m_prevFacing{};
		Protobuf::WEAPON_TYPE m_prevWeapon{};
		Shared::vec4 m_prevColor{};

		bool m_prevInvincible = false;
		bool m_isBlinkVisible = true;
		float m_blinkElapsedTime = 0.0f;
		float m_blinkInterval = 0.1f;
	};
}

