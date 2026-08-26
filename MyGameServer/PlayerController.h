#pragma once
#include "ClientBehaviour.h"
#include "INetworkState.h"

namespace MGSL::Server
{
	MGSL_CLASS_PTR(GameObject)
	MGSL_CLASS_PTR(PlayerController)
	MGSL_CLASS_PTR(CharacterBody2D)
	MGSL_CLASS_PTR(BoxCollider)

	/*===============================//
	//   PlayerController on sever   //
	//===============================*/
	class PlayerController : public ClientBehaviour, public INetworkState<::Protobuf::PlayerInfo>
	{
		MGSL_DISABLE_COPY(PlayerController)
		MGSL_DISABLE_MOVE(PlayerController)
		using Super = ClientBehaviour;

	public:
		virtual ~PlayerController() override;
		static PlayerControllerUPtr Create(GameObject* owner);

	/*========================================//
	//      Default Controller Methods        //
	//========================================*/
	public:
		virtual void Update(float deltaTime) override;
		virtual void OnTriggerEnter(BoxCollider* other) override;
		virtual void OnTriggerStay(BoxCollider* other) override;
		virtual void OnTriggerExit(BoxCollider* other) override;

	private:
		explicit PlayerController(GameObject* owner);

	/*==============================================//
	//      Default player syncing behaviours       //
	//==============================================*/

#pragma region DEATH
	/*========================//
	//          Death         //
	//========================*/
	public:
		void Death();
		bool IsDead() const { return m_isDead; }

	private:
		bool UpdateDeath(float deltaTime);

	private:
		bool m_isDead = false;
#pragma endregion

#pragma region HIT
	/*======================//
	//          Hit         //
	//======================*/
	public:
		void Hit();
		void TakeDamage(Shared::uint32 damage, PlayerController* attacker);
		Shared::uint32 GetLife() const { return m_info.life(); }
		bool IsInvincible() const { return m_info.invincible(); }
		Shared::uint32 GetKillCount() const { return m_info.kill_cnt(); }
		void AddKill() { m_info.set_kill_cnt(m_info.kill_cnt() + 1); }

	private:
		void UpdateHit(float deltaTime);

	private:
		bool m_isHit = false;
		float m_hitElapsedTime = 0.0f;
		float m_hitDuration = 0.3f;
		float m_invincibleElapsedTime = 0.0f;
		float m_invincibleDuration = 1.5f;

#pragma endregion

#pragma region LAND
	private:
		void UpdateLand(CharacterBody2D* body, float deltaTime);
		bool IsLanding() const { return m_isLanding; }

	private:
		bool m_wasGrounded = true;
		bool m_isLanding = false;
		float m_landElapsedTime = 0.0f;
		float m_landDuration = 0.15f;
#pragma endregion

#pragma region MOVE
	/*========================//
	//          Move          //
	//========================*/
	public:
		void SetHorizontalDirection(::Protobuf::DIR_TYPE dir) { m_horizontalDirection = dir; }
		void SetVerticalDirection(::Protobuf::DIR_TYPE dir) { m_verticalDirection = dir; }
		::Protobuf::DIR_TYPE GetHorizontalDirection() const { return m_horizontalDirection; }
		::Protobuf::DIR_TYPE GetVerticalDirection() const { return m_verticalDirection; }
		void SetRunning(bool running) { m_isRunning = running; }

	private:
		void UpdateMove(GameObject* owner, float directionX, float deltaTime);
		float GetHorizontalDirectionValue();
		float GetVerticalDirectionValue();

	private:
		::Protobuf::DIR_TYPE m_horizontalDirection = ::Protobuf::DIR_TYPE_NONE;
		::Protobuf::DIR_TYPE m_verticalDirection = ::Protobuf::DIR_TYPE_NONE;
		float m_moveSpeed = 3.0f;
		float m_runSpeed = 4.5f;
		bool m_isRunning = false;
#pragma endregion

#pragma region CLIMB
	/*========================//
	//          Climb         //
	//========================*/
	private:
		void UpdateClimb(CharacterBody2D* body, float directionY);
		bool IsClimbing() const { return m_ladderState == ELadderState::CLIMBING; }

	private:
		float m_climbSpeed = 2.5f;
		ELadderState m_ladderState = ELadderState::NONE;

#pragma endregion

#pragma region ATTACK
	/*========================//
	//          Attack        //
	//========================*/
	public:
		void Attack();
		bool IsAttacking() const { return m_isAttacking; }

	private:
		bool UpdateAttack(float deltaTime);
		bool RegisterHitTarget(Shared::uint64 objectID);

	private:
		bool m_isAttacking = false;
		bool m_attackQueued = false;
		Shared::uint32 m_comboIndex = 0;
		float m_attackElapsedTime = 0.0f;
		float m_attackDuration = 0.3f;
		float m_comboWindowStart = 0.20f;
		float m_comboWindowEnd = 0.27f;
		Shared::HashSet<Shared::uint64> m_hitTargets;
#pragma endregion

#pragma region COLLISION
	/*======================//
	//   Trigger Handlers   //
	//======================*/
	private:
		void HandleTrigger(BoxCollider* other);

#pragma endregion

#pragma region PLAYER_STATE
	/*========================//
	//      Player State      //
	//========================*/
	private:
		void UpdatePlayerState(CharacterBody2D* body, float directionX);

	public:
		void SetWeapon(Protobuf::WEAPON_TYPE weapon) { m_info.set_weapon(weapon); }
		Protobuf::OBJECT_STATE_TYPE GetState() const { return m_info.state(); }
		Protobuf::FACING_TYPE GetFacing() const { return m_info.facing(); }
		Protobuf::WEAPON_TYPE GetWeapon() const { return m_info.weapon(); }
#pragma endregion		

#pragma region NETWORK_STATE_INTERFACE_IMPL
	/*=======================================//
	//   INetworkState interface overrides   //
	//=======================================*/
	public:
		virtual Shared::uint64 GetObjectID() const					   override { return m_info.objectid(); }
		virtual void SetObjectID(Shared::uint64 objectID)			   override { m_info.set_objectid(objectID); }
		virtual void SetInfo(const ::Protobuf::PlayerInfo& info)	   override { m_info = info; }
		virtual void SetInfo(::Protobuf::PlayerInfo&& info)			   override { m_info = std::move(info); }
		virtual ::Protobuf::PlayerInfo& GetInfo() noexcept             override { return m_info; }
		virtual const ::Protobuf::PlayerInfo& GetInfo() const noexcept override { return m_info; }

	private:
		::Protobuf::PlayerInfo m_info;
#pragma endregion
	};
}
