#include "2DPch.h"
#include "MyPlayerStateMachine.h"
#include "MyPlayerNetworkState.h"
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
		m_playerNetworkState = GetOwner()->GetComponent<MyPlayerNetworkState>();
	}

	void MyPlayerStateMachine::Update(float /*deltaTime*/)
	{
		if (!m_playerNetworkState || !m_flipbookPlayer) return;
		if (m_prevState != m_playerNetworkState->GetState()) ApplyState();
		if (m_prevFacing != m_playerNetworkState->GetFacing()) ApplyFacing();
		if (m_prevWeapon != m_playerNetworkState->GetWeapon()) ApplyWeapon();
		if (m_prevColor != m_playerNetworkState->GetColor()) ApplyColor();
	}

	void MyPlayerStateMachine::ApplyState()
	{
		const Protobuf::OBJECT_STATE_TYPE state = m_playerNetworkState->GetState();
		m_prevState = state;
		m_flipbookPlayer->SetState(static_cast<Shared::uint32>(state));
	}

	void MyPlayerStateMachine::ApplyFacing()
	{
		const Protobuf::FACING_TYPE facing = m_playerNetworkState->GetFacing();
		m_prevFacing = facing;
		const bool flipX = facing == Protobuf::FACING_TYPE_LEFT;
		m_flipbookPlayer->SetFlipX(flipX);
	}

	void MyPlayerStateMachine::ApplyWeapon()
	{
		const Protobuf::WEAPON_TYPE weapon = m_playerNetworkState->GetWeapon();
		const Shared::uint32 controllerIndex = static_cast<Shared::uint32>(weapon);
		if (!m_flipbookPlayer->ChangeController(controllerIndex)) return;
		m_prevWeapon = weapon;
		m_flipbookPlayer->SetState(static_cast<Shared::uint32>(m_playerNetworkState->GetState()));
	}

	void MyPlayerStateMachine::ApplyColor()
	{
		const Shared::vec4 color = m_playerNetworkState->GetColor();
		m_prevColor = color;
		m_flipbookPlayer->SetColor(color);
	}
}