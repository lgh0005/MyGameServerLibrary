#include "2DPch.h"
#include "MyPlayerStateMachine.h"
#include "MyGameFramework/GameObject.h"
#include "MyGameFramework/FlipbookPlayer.h"

namespace MGSL::Sandbox2D
{
	MyPlayerStateMachine::MyPlayerStateMachine(Framework::GameObject* owner) : Super(owner) { }
	MyPlayerStateMachine::~MyPlayerStateMachine() = default;

	MyPlayerStateMachineUPtr MyPlayerStateMachine::Create(Framework::GameObject* owner)
	{
		return MyPlayerStateMachineUPtr(new MyPlayerStateMachine(owner));
	}

	void MyPlayerStateMachine::Awake()
	{
		m_flipbookPlayer = GetOwner()->GetComponent<Framework::FlipbookPlayer>();
	}

	void MyPlayerStateMachine::SetState(Protobuf::OBJECT_STATE_TYPE state)
	{
		if (m_state == state) return;
		m_state = state;
		m_flipbookPlayer->SetState(static_cast<Shared::uint32>(m_state));
	}

	void MyPlayerStateMachine::SetFacing(Protobuf::FACING_TYPE facing)
	{
		if (m_facing == facing) return;
		m_facing = facing;
		
		if (!m_flipbookPlayer) return;
		const bool flipX = m_facing == Protobuf::FACING_TYPE_LEFT;
		m_flipbookPlayer->SetFlipX(flipX);
	}

	void MyPlayerStateMachine::SetWeapon(Protobuf::WEAPON_TYPE weapon)
	{
		if (m_weapon == weapon) return;
		m_weapon = weapon;

		const Shared::uint32 controllerIndex = static_cast<Shared::uint32>(m_weapon);
		if (!m_flipbookPlayer->ChangeController(controllerIndex)) return;
		m_flipbookPlayer->SetState(static_cast<Shared::uint32>(m_state));
	}

	Protobuf::OBJECT_STATE_TYPE MyPlayerStateMachine::GetState() const
	{
		return m_state;
	}

	Protobuf::FACING_TYPE MyPlayerStateMachine::GetFacing() const
	{
		return m_facing;
	}

	Protobuf::WEAPON_TYPE MyPlayerStateMachine::GetWeapon() const
	{
		return m_weapon;
	}
}