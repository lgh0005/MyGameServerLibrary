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
		virtual void OnCollisionEnter(BoxCollider* other) override;
		virtual void OnCollisionStay(BoxCollider* other) override;
		virtual void OnCollisionExit(BoxCollider* other) override;

	public:
		void SetMoveDirection(::Protobuf::DIR_TYPE dir);
		Protobuf::DIR_TYPE GetMoveDirection() const;

	private:
		PlayerController(GameObject* owner);

		Protobuf::DIR_TYPE m_moveDirection = ::Protobuf::DIR_TYPE_NONE;
		float m_moveSpeed = 3.0f;
	};
}

