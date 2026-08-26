#include "2DPch.h"
#include "MyPlayerController.h"
#include "MyGameFramework/MouseDevice.h"
#include "MyGameFramework/KeyboardDevice.h"
#include "MyGameFramework/CharacterBody2D.h"
#include "MyPlayerNetworkState.h"
#include "ClientPacketHandler.h"

namespace MGSL::Sandbox2D
{
	MyPlayerController::MyPlayerController(Framework::GameObject* owner) : Super(owner) { }
	MyPlayerController::~MyPlayerController() = default;

	/*========================================//
	//   Default MyPlayerController Methods   //
	//========================================*/
	MyPlayerControllerUPtr MyPlayerController::Create(Framework::GameObject* owner)
	{
		return MyPlayerControllerUPtr(new MyPlayerController(owner));
	}

	void MyPlayerController::Awake()
	{
		m_playerNetworkState = GetOwner()->GetComponent<MyPlayerNetworkState>();
		m_characterBody = GetOwner()->GetComponent<Framework::CharacterBody2D>();
	}

	void MyPlayerController::Update(float deltaTime)
	{
		// 죽었는 지 판정
		if (Dead()) return;

		// 이동과 공격 인풋 처리
		HandleMovementInput(MGSL_INPUT_MGR.GetKeyboard());
		HandleWeaponInput(MGSL_INPUT_MGR.GetKeyboard());
		HandleAttackInput(MGSL_INPUT_MGR.GetMouse());
	}

	/*=========================//
	//   Behaviour Handlers    //
	//=========================*/
	void MyPlayerController::HandleMovementInput(Framework::KeyboardDevice* keyboard)
	{
		Move(keyboard);
		Jump(keyboard);
		Dash(keyboard);
		Slide(keyboard);
	}

	void MyPlayerController::HandleWeaponInput(Framework::KeyboardDevice* keyboard)
	{
		/*========================//
		//      Weapon Change     //
		//========================*/
		if (keyboard->GetKeyDown('1')) SendChangeWeaponPacket(::Protobuf::WEAPON_TYPE_NONE);
		if (keyboard->GetKeyDown('2')) SendChangeWeaponPacket(::Protobuf::WEAPON_TYPE_PISTOL);
		if (keyboard->GetKeyDown('3')) SendChangeWeaponPacket(::Protobuf::WEAPON_TYPE_SWORD);
	}

	void MyPlayerController::HandleAttackInput(Framework::MouseDevice* mouse)
	{
		if (mouse->GetButtonDown(Framework::EMouseButton::LEFT)) SendAttackPacket();
	}

#pragma region MY_PLAYER_BEHAVIOURS
	/*================//
	//   Behaviours   //
	//================*/
	void MyPlayerController::Move(Framework::KeyboardDevice* keyboard)
	{
		float directionX = 0.0f;
		::Protobuf::DIR_TYPE horizontalDir = ::Protobuf::DIR_TYPE_NONE;
		::Protobuf::DIR_TYPE verticalDir = ::Protobuf::DIR_TYPE_NONE;

		// Move
		if (keyboard->GetKeyPress('A')) { directionX = -1.0f; horizontalDir = ::Protobuf::DIR_TYPE_LEFT; }
		else if (keyboard->GetKeyPress('D')) { directionX = 1.0f; horizontalDir = ::Protobuf::DIR_TYPE_RIGHT; }
		if (keyboard->GetKeyPress('W')) { verticalDir = ::Protobuf::DIR_TYPE_UP; }
		else if (keyboard->GetKeyPress('S')) { verticalDir = ::Protobuf::DIR_TYPE_DOWN; }

		// Run
		const bool isRunning = keyboard->GetKeyPress(VK_SHIFT);
		const float moveSpeed = isRunning ? m_runSpeed : m_moveSpeed;
		m_characterBody->SetHorizontalVelocity(directionX * moveSpeed);

		// prev state settings and packet sending
		if (horizontalDir != m_prevHorizontalDir ||
			verticalDir != m_prevVerticalDir     ||
			isRunning != m_prevRunning)
		{
			SendMovePacket(horizontalDir, verticalDir, isRunning);
			m_prevHorizontalDir = horizontalDir;
			m_prevVerticalDir = verticalDir;
			m_prevRunning = isRunning;
		}
	}

	void MyPlayerController::Jump(Framework::KeyboardDevice* keyboard)
	{
		if (!keyboard->GetKeyDown(VK_SPACE)) return;
		m_characterBody->Jump(m_jumpPower);
		SendJumpPacket();
	}

	void MyPlayerController::Dash(Framework::KeyboardDevice* keyboard)
	{
		// TODO : aa 또는 dd를 연타할 경우, body를 해당 방향으로 AddForce
	}

	void MyPlayerController::Slide(Framework::KeyboardDevice* keyboard)
	{
		// TODO : sa 또는 sd를 누를 경우, 눕는 상태로 전환되면서 AddForce
	}

	bool MyPlayerController::Dead()
	{
		if (m_playerNetworkState->GetState() != Protobuf::OBJECT_STATE_TYPE_DEATH)
			return false;

		m_characterBody->SetHorizontalVelocity(0.0f);
		m_characterBody->SetVerticalVelocity(0.0f);
		return true;
	}
#pragma endregion

#pragma region PACKET_SENDINGS
	/*========================//
	//   Packet Test Methods  //
	//========================*/
	void MyPlayerController::SendMovePacket(::Protobuf::DIR_TYPE horizontalDir, ::Protobuf::DIR_TYPE verticalDir, bool running)
	{
		auto sendBuffer = Net::ClientPacketHandler::Make_C_Move(horizontalDir, verticalDir, running);
		MGSL_NETWORK_MGR.SendPacket(sendBuffer);
	}

	void MyPlayerController::SendJumpPacket()
	{
		auto sendBuffer = Net::ClientPacketHandler::Make_C_Jump();
		MGSL_NETWORK_MGR.SendPacket(sendBuffer);
	}

	void MyPlayerController::SendChangeWeaponPacket(::Protobuf::WEAPON_TYPE weapon)
	{
		auto sendBuffer = Net::ClientPacketHandler::Make_C_ChangeWeapon(weapon);
		MGSL_NETWORK_MGR.SendPacket(sendBuffer);
	}

	void MyPlayerController::SendAttackPacket()
	{
		auto sendBuffer = Net::ClientPacketHandler::Make_C_Attack();
		MGSL_NETWORK_MGR.SendPacket(sendBuffer);
	}
#pragma endregion
}
