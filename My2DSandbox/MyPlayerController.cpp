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
		/*=====================//
		//   Is status dead?   //
		//=====================*/
		if (m_playerNetworkState->GetState() == Protobuf::OBJECT_STATE_TYPE_DEATH)
		{
			m_characterBody->SetHorizontalVelocity(0.0f);
			return;
		}

		// 이동과 공격 인풋 처리
		HandleMovementInput(deltaTime);
		HandleAttackInput();
	}

	void MyPlayerController::HandleMovementInput(float deltaTime)
	{
		auto keyboard = MGSL_INPUT_MGR.GetKeyboard();
		if (!m_characterBody) return;

		/*========================//
		//   Horizontal Movement  //
		//========================*/
		float directionX = 0.0f;
		if (keyboard->GetKeyPress('A')) directionX -= 1.0f;
		if (keyboard->GetKeyPress('D')) directionX += 1.0f;
		const bool isRunning = keyboard->GetKeyPress(VK_SHIFT);
		const float moveSpeed = isRunning ? m_runSpeed : m_moveSpeed;
		m_characterBody->SetHorizontalVelocity(directionX * moveSpeed);
		
		/*========================//
		//          Jump          //
		//========================*/
		if (keyboard->GetKeyDown(VK_SPACE))
		{
			m_characterBody->Jump(m_jumpPower);
			SendJumpPacket();
		}

		/*========================//
		//      Weapon Change     //
		//========================*/
		if (keyboard->GetKeyDown('1')) SendChangeWeaponPacket(::Protobuf::WEAPON_TYPE_NONE);
		if (keyboard->GetKeyDown('2')) SendChangeWeaponPacket(::Protobuf::WEAPON_TYPE_PISTOL);
		if (keyboard->GetKeyDown('3')) SendChangeWeaponPacket(::Protobuf::WEAPON_TYPE_SWORD);

		/*========================//
	    //     Direction Input    //
	    //========================*/
		::Protobuf::DIR_TYPE horizontalDir = ::Protobuf::DIR_TYPE_NONE;
		::Protobuf::DIR_TYPE verticalDir = ::Protobuf::DIR_TYPE_NONE;
		if (directionX < 0.0f) horizontalDir = ::Protobuf::DIR_TYPE_LEFT;
		else if (directionX > 0.0f) horizontalDir = ::Protobuf::DIR_TYPE_RIGHT;
		if (keyboard->GetKeyPress('W')) verticalDir = ::Protobuf::DIR_TYPE_UP;
		else if (keyboard->GetKeyPress('S')) verticalDir = ::Protobuf::DIR_TYPE_DOWN;

		/*========================//
		//      Move Packet       //
		//========================*/
		if (horizontalDir != m_prevHorizontalDir ||
			verticalDir != m_prevVerticalDir ||
			isRunning != m_prevRunning)
		{
			SendMovePacket(horizontalDir, verticalDir, isRunning);
			m_prevHorizontalDir = horizontalDir;
			m_prevVerticalDir = verticalDir;
			m_prevRunning = isRunning;
		}
	}

	void MyPlayerController::HandleAttackInput()
	{
		auto mouse = MGSL_INPUT_MGR.GetMouse();
		if (mouse->GetButtonDown(Framework::EMouseButton::LEFT))
			SendAttackPacket();
	}

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
}
