#pragma once
#include "MyGameFramework/MonoBehaviour.h"

namespace MGSL::Framework { MGSL_CLASS_PTR(CharacterBody2D) }

namespace MGSL::Sandbox2D
{
	MGSL_CLASS_PTR(MyPlayerController)

	/*===============================//
	//   PlayerController on client  //
	//===============================*/
	class MyPlayerController : public Framework::MonoBehaviour
	{
		using Super = Framework::MonoBehaviour;

	public:
		virtual ~MyPlayerController() override;
		static MyPlayerControllerUPtr Create(Framework::GameObject* owner);

	public:
		virtual void Awake() override;
		virtual void Update(float deltaTime) override;

	private:
		explicit MyPlayerController(Framework::GameObject* owner);
		void HandleMovementInput(float deltaTime);
		void HandleAttackInput();

		// 이동 처리
		bool m_prevRunning = false;
		float m_moveSpeed = 3.0f;
		float m_runSpeed = 4.5f;
		float m_jumpPower = 3.5f;
		::Protobuf::DIR_TYPE m_prevMoveDir = ::Protobuf::DIR_TYPE_NONE;
		Framework::CharacterBody2D* m_characterBody = nullptr;

	/*========================//
	//   Packet Test Methods  //
	//========================*/
	private:
		void SendMovePacket(::Protobuf::DIR_TYPE dir, bool running);
		void SendJumpPacket();
		void SendChangeWeaponPacket(::Protobuf::WEAPON_TYPE weapon);
		void SendAttackPacket();
	};
}

