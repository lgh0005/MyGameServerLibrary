#pragma once
#include "Component.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(BoxCollider)

	class MonoBehaviour : public Component
	{
		using Super = Component;

	public:
		virtual ~MonoBehaviour();

	public:
		virtual void OnCollisionEnter(BoxCollider* other);
		virtual void OnCollisionStay(BoxCollider* other);
		virtual void OnCollisionExit(BoxCollider* other);

		virtual void OnTriggerEnter(BoxCollider* other);
		virtual void OnTriggerStay(BoxCollider* other);
		virtual void OnTriggerExit(BoxCollider* other);

	protected:
		MonoBehaviour(GameObject* owner);
	};
}

