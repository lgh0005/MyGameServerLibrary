#pragma once
#include "MyGameFramework/MonoBehaviour.h"

namespace MGSL::Framework { MGSL_CLASS_PTR(FlipbookPlayer) }

namespace MGSL::Sandbox2D
{
	MGSL_CLASS_PTR(UIController)
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

	public:
		void SetState(Protobuf::OBJECT_STATE_TYPE state);
		void SetFacing(Protobuf::FACING_TYPE facing);
		void SetWeapon(Protobuf::WEAPON_TYPE weapon);

	public:
		Protobuf::OBJECT_STATE_TYPE GetState() const;
		Protobuf::FACING_TYPE GetFacing() const;
		Protobuf::WEAPON_TYPE GetWeapon() const;

	public:
		void SetUIController(UIController* uiController);
		void SetPlayerColor(const Shared::vec4& color);

	private:
		explicit MyPlayerStateMachine(Framework::GameObject* owner);
		
		// 연동할 UIController 핸들
		UIController* m_uiController = nullptr;
		void UpdateWeaponUI();

		Framework::FlipbookPlayer* m_flipbookPlayer = nullptr;
		Protobuf::OBJECT_STATE_TYPE m_state = Protobuf::OBJECT_STATE_TYPE_IDLE;
		Protobuf::FACING_TYPE m_facing = Protobuf::FACING_TYPE_RIGHT;
		Protobuf::WEAPON_TYPE m_weapon = Protobuf::WEAPON_TYPE_NONE;
	};
}

