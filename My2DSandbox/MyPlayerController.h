#pragma once
#include "MyGameFramework/MonoBehaviour.h"

namespace MGSL::Framework 
{ 
	MGSL_CLASS_PTR(KeyboardDevice)
	MGSL_CLASS_PTR(MouseDevice)
	MGSL_CLASS_PTR(CharacterBody2D) 
}

namespace MGSL::Sandbox2D
{
	MGSL_CLASS_PTR(MyPlayerNetworkState)
	MGSL_CLASS_PTR(MyPlayerController)

	/*===============================//
	//   PlayerController on client  //
	//===============================*/
	class MyPlayerController : public Framework::MonoBehaviour
	{
		using Super = Framework::MonoBehaviour;

	/*========================================//
	//   Default MyPlayerController Methods   //
	//========================================*/
	public:
		virtual ~MyPlayerController() override;
		static MyPlayerControllerUPtr Create(Framework::GameObject* owner);
		virtual void Awake() override;
		virtual void Update(float deltaTime) override;

	/*=========================//
	//   Behaviour Handlers    //
	//=========================*/
	private:
		void HandleMovementInput(Framework::KeyboardDevice* keyboard);
		void HandleWeaponInput(Framework::KeyboardDevice* keyboard);
		void HandleAttackInput(Framework::MouseDevice* mouse);

	/*================//
	//   Behaviours   //
	//================*/
	private:
		void Move(Framework::KeyboardDevice* keyboard);
		void Jump(Framework::KeyboardDevice* keyboard);
		bool Dead();

	private:
		void UpdateHitEffect(float deltaTime);
		void PlayHitEffect();

	private:
		explicit MyPlayerController(Framework::GameObject* owner);

		// 상태 조회
		MyPlayerNetworkState* m_playerNetworkState = nullptr;

		// 이동 처리
		float m_moveSpeed = 3.0f;
		float m_runSpeed = 4.5f;
		float m_jumpPower = 3.5f;
		bool m_prevRunning = false;
		::Protobuf::DIR_TYPE m_prevHorizontalDir = ::Protobuf::DIR_TYPE_NONE;
		::Protobuf::DIR_TYPE m_prevVerticalDir = ::Protobuf::DIR_TYPE_NONE;
		Framework::CharacterBody2D* m_characterBody = nullptr;

		// 피격 포스트 프로세싱
		::Protobuf::OBJECT_STATE_TYPE m_prevState = ::Protobuf::OBJECT_STATE_TYPE_IDLE;
		float m_hitEffectElapsedTime = 0.0f;
		float m_hitEffectDuration = 1.0f;
		bool m_isHitEffectPlaying = false;

#pragma region PACKET_SENDINGS
	/*========================//
	//   Packet Test Methods  //
	//========================*/
	private:
		void SendMovePacket(::Protobuf::DIR_TYPE horizontalDir, ::Protobuf::DIR_TYPE verticalDir, bool running);
		void SendJumpPacket();
		void SendChangeWeaponPacket(::Protobuf::WEAPON_TYPE weapon);
		void SendAttackPacket();
	};
#pragma endregion
}

