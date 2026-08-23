#include "ServerPch.h"
#include "ClientBehaviour.h"
#include "BoxCollider.h"

namespace MGSL::Server
{
	ClientBehaviour::ClientBehaviour(GameObject* owner) : Super(owner) {}
	ClientBehaviour::~ClientBehaviour() = default;

	void ClientBehaviour::OnCollisionEnter(BoxCollider* other) {}
	void ClientBehaviour::OnCollisionStay(BoxCollider* other) {}
	void ClientBehaviour::OnCollisionExit(BoxCollider* other) {}

	void ClientBehaviour::OnTriggerEnter(BoxCollider* other) {}
	void ClientBehaviour::OnTriggerStay(BoxCollider* other) {}
	void ClientBehaviour::OnTriggerExit(BoxCollider* other) {}
}
