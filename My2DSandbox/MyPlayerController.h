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

		// TODO : 
		// 1. 애니메이션 상태 처리
		// 2. 공격 처리
		// 3. 죽음 처리

		// 이동 처리
		float m_moveSpeed = 3.0f;
		float m_jumpPower = 3.5f;
		::Protobuf::DIR_TYPE m_prevMoveDir = ::Protobuf::DIR_TYPE_NONE;
		Framework::CharacterBody2D* m_characterBody = nullptr;

	/*========================//
	//   Packet Test Methods  //
	//========================*/
	private:
		void SendMovePacket(::Protobuf::DIR_TYPE dir);
		void SendJumpPacket();
	};
}

