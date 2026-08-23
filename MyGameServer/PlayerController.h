#pragma once
#include "ClientBehaviour.h"

namespace MGSL::Server
{
	MGSL_CLASS_PTR(GameObject)
	MGSL_CLASS_PTR(PlayerController)

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

	public:
		void SetMoveDirection(::Protobuf::DIR_TYPE dir);
		void SetWeapon(Protobuf::WEAPON_TYPE weapon);
		Protobuf::DIR_TYPE GetMoveDirection() const;
		Protobuf::OBJECT_STATE_TYPE GetState() const;
		Protobuf::FACING_TYPE GetFacing() const;
		Protobuf::WEAPON_TYPE GetWeapon() const;

	private:
		PlayerController(GameObject* owner);

		float m_moveSpeed = 3.0f;
		Protobuf::DIR_TYPE m_moveDirection = ::Protobuf::DIR_TYPE_NONE;
		Protobuf::OBJECT_STATE_TYPE m_state = Protobuf::OBJECT_STATE_TYPE_IDLE;
		Protobuf::FACING_TYPE m_facing = Protobuf::FACING_TYPE_RIGHT;
		Protobuf::WEAPON_TYPE m_weapon = Protobuf::WEAPON_TYPE_NONE;

	// DEBUG
	private:
		Protobuf::OBJECT_STATE_TYPE m_prevState = Protobuf::OBJECT_STATE_TYPE_IDLE;
		Protobuf::FACING_TYPE m_prevFacing = Protobuf::FACING_TYPE_RIGHT;
	};
}

