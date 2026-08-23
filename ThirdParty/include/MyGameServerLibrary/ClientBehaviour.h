#pragma once
#include "Component.h"

namespace MGSL::Server
{
	MGSL_CLASS_PTR(BoxCollider)

	class ClientBehaviour : public Component
	{
		MGSL_DISABLE_COPY(ClientBehaviour)
		MGSL_DISABLE_MOVE(ClientBehaviour)
		using Super = Component;

	public:
		virtual ~ClientBehaviour();

	public:
		virtual void OnCollisionEnter(BoxCollider* other);
		virtual void OnCollisionStay(BoxCollider* other);
		virtual void OnCollisionExit(BoxCollider* other);

		virtual void OnTriggerEnter(BoxCollider* other);
		virtual void OnTriggerStay(BoxCollider* other);
		virtual void OnTriggerExit(BoxCollider* other);

	protected:
		ClientBehaviour(GameObject* owner);
	};
}

