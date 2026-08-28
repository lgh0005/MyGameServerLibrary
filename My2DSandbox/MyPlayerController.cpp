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
		if (m_playerNetworkState) m_prevState = m_playerNetworkState->GetState();
	}

	void MyPlayerController::Update(float deltaTime)
	{
		// 피격 포스트 프로세싱
		UpdateHitEffect(deltaTime);

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

	bool MyPlayerController::Dead()
	{
		if (m_playerNetworkState->GetState() != Protobuf::OBJECT_STATE_TYPE_DEATH)
			return false;

		m_characterBody->SetHorizontalVelocity(0.0f);
		m_characterBody->SetVerticalVelocity(0.0f);
		return true;
	}




	void MyPlayerController::PlayHitEffect()
	{
		m_hitEffectElapsedTime = 0.0f;
		m_isHitEffectPlaying = true;
		MGSL_RENDER_MGR.SetVignetteIntensity(0.8f);
		MGSL_RENDER_MGR.SetChromaticAberrationStrength(0.05f);
	}

	void MyPlayerController::UpdateHitEffect(float deltaTime)
	{
		if (!m_playerNetworkState) return;

		const ::Protobuf::OBJECT_STATE_TYPE currentState = m_playerNetworkState->GetState();

		// HIT 상태로 처음 진입했을 때만 발동
		if (currentState == ::Protobuf::OBJECT_STATE_TYPE_HIT &&
			m_prevState != ::Protobuf::OBJECT_STATE_TYPE_HIT)
			PlayHitEffect();

		m_prevState = currentState;

		if (!m_isHitEffectPlaying) return;

		m_hitEffectElapsedTime += deltaTime;

		float t = m_hitEffectElapsedTime / m_hitEffectDuration;
		t = glm::clamp(t, 0.0f, 1.0f);

		const float vignetteIntensity = glm::mix(0.8f, 0.0f, t);
		const float chromaticAberrationStrength = glm::mix(0.05f, 0.0075f, t);
		MGSL_RENDER_MGR.SetVignetteIntensity(vignetteIntensity);
		MGSL_RENDER_MGR.SetChromaticAberrationStrength(chromaticAberrationStrength);

		if (t >= 1.0f)
		{
			m_isHitEffectPlaying = false;
			m_hitEffectElapsedTime = 0.0f;
			MGSL_RENDER_MGR.SetVignetteIntensity(0.0f);
			MGSL_RENDER_MGR.SetChromaticAberrationStrength(0.0075f);
		}
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
