#include "2DPch.h"
#include "MyPlayerController.h"
#include "MyGameFramework/KeyboardDevice.h"
#include "MyGameFramework/CharacterBody2D.h"
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
		m_characterBody = GetOwner()->GetComponent<Framework::CharacterBody2D>();
	}

	void MyPlayerController::Update(float deltaTime)
	{
		HandleMovementInput(deltaTime);
	}

	void MyPlayerController::HandleMovementInput(float deltaTime)
	{
		auto keyboard = MGSL_INPUT_MGR.GetKeyboard();
		Framework::CharacterBody2D* body = GetOwner()->GetComponent<Framework::CharacterBody2D>();
		if (!body) return;

		/*========================//
		//   Horizontal Movement  //
		//========================*/
		float directionX = 0.0f;
		if (keyboard->GetKeyPress('A')) directionX -= 1.0f;
		if (keyboard->GetKeyPress('D')) directionX += 1.0f;
		const bool isRunning = keyboard->GetKeyPress(VK_SHIFT);
		const float moveSpeed = isRunning ? m_runSpeed : m_moveSpeed;
		body->SetHorizontalVelocity(directionX * moveSpeed);
		
		/*========================//
		//          Jump          //
		//========================*/
		if (keyboard->GetKeyDown(VK_SPACE))
		{
			body->Jump(m_jumpPower);
			SendJumpPacket();
		}

		/*========================//
		//      Weapon Change     //
		//========================*/
		if (keyboard->GetKeyDown('1')) SendChangeWeaponPacket(::Protobuf::WEAPON_TYPE_NONE);
		if (keyboard->GetKeyDown('2')) SendChangeWeaponPacket(::Protobuf::WEAPON_TYPE_PISTOL);
		if (keyboard->GetKeyDown('3')) SendChangeWeaponPacket(::Protobuf::WEAPON_TYPE_SWORD);

		/*========================//
		//      Move Packet       //
		//========================*/
		::Protobuf::DIR_TYPE moveDir = ::Protobuf::DIR_TYPE_NONE;
		if (directionX < 0.0f) moveDir = ::Protobuf::DIR_TYPE_LEFT;
		else if (directionX > 0.0f) moveDir = ::Protobuf::DIR_TYPE_RIGHT;
		if (moveDir != m_prevMoveDir || isRunning != m_prevRunning)
		{
			SendMovePacket(moveDir, isRunning);
			m_prevMoveDir = moveDir;
			m_prevRunning = isRunning;
		}
	}

	/*========================//
	//   Packet Test Methods  //
	//========================*/
	void MyPlayerController::SendMovePacket(::Protobuf::DIR_TYPE dir, bool running)
	{
		auto sendBuffer = Net::ClientPacketHandler::Make_C_Move(dir, running);
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
}
