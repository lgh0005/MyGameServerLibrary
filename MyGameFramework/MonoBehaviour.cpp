#include "GameFrameworkPch.h"
#include "MonoBehaviour.h"
#include "BoxCollider.h"

namespace MGSL::Framework
{
	MonoBehaviour::MonoBehaviour(GameObject* owner) : Super(owner) { }
	MonoBehaviour::~MonoBehaviour() = default;

	void MonoBehaviour::OnCollisionEnter(BoxCollider* other) { }
	void MonoBehaviour::OnCollisionStay(BoxCollider* other) { }
	void MonoBehaviour::OnCollisionExit(BoxCollider* other) { }

	void MonoBehaviour::OnTriggerEnter(BoxCollider* other) { }
	void MonoBehaviour::OnTriggerStay(BoxCollider* other) { }
	void MonoBehaviour::OnTriggerExit(BoxCollider* other) { }
}
