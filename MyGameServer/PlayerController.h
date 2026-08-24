#pragma once
#include "ClientBehaviour.h"

namespace MGSL::Server
{
	MGSL_CLASS_PTR(GameObject)
	MGSL_CLASS_PTR(PlayerController)
	MGSL_CLASS_PTR(BoxCollider)

	/*===============================//
	//   PlayerController on sever   //
	//===============================*/
	class PlayerController : public ClientBehaviour
	{
		MGSL_DISABLE_COPY(PlayerController)
		MGSL_DISABLE_MOVE(PlayerController)
		using Super = ClientBehaviour;

	public:
		virtual ~PlayerController() override;
		static PlayerControllerUPtr Create(GameObject* owner);

	public:
		virtual void Update(float deltaTime) override;
		virtual void OnTriggerEnter(BoxCollider* other) override;
		virtual void OnTriggerStay(BoxCollider* other) override;
		void Attack();

	private:
		void HandleHitboxTrigger(BoxCollider* other);
		bool RegisterHitTarget(Shared::uint64 objectID);

	public:
		void SetMoveDirection(::Protobuf::DIR_TYPE dir);
		void SetRunning(bool running);
		void SetWeapon(Protobuf::WEAPON_TYPE weapon);

	public:
		Protobuf::DIR_TYPE GetMoveDirection() const;
		Protobuf::OBJECT_STATE_TYPE GetState() const;
		Protobuf::FACING_TYPE GetFacing() const;
		Protobuf::WEAPON_TYPE GetWeapon() const;
		bool IsAttacking() const;

	private:
		PlayerController(GameObject* owner);

		// 플레이어 상태
		Protobuf::DIR_TYPE m_moveDirection = ::Protobuf::DIR_TYPE_NONE;
		Protobuf::OBJECT_STATE_TYPE m_state = Protobuf::OBJECT_STATE_TYPE_IDLE;
		Protobuf::FACING_TYPE m_facing = Protobuf::FACING_TYPE_RIGHT;
		Protobuf::WEAPON_TYPE m_weapon = Protobuf::WEAPON_TYPE_NONE;

		// 이동 관련 멤버
		float m_moveSpeed = 3.0f;
		float m_runSpeed = 4.5f;
		bool m_isRunning = false;

		// 공격 관련 멤버
		bool m_isAttacking = false;
		bool m_attackQueued = false;
		Shared::uint32 m_comboIndex = 0;
		float m_attackElapsedTime = 0.0f;
		float m_attackDuration = 0.3f;
		float m_comboWindowStart = 0.20f;
		float m_comboWindowEnd = 0.27f;
		Shared::HashSet<Shared::uint64> m_hitTargets;
	};
}

