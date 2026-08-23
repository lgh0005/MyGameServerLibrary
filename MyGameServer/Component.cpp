#include "ServerPch.h"
#include "Component.h"
#include "GameObject.h"

namespace MGSL::Server
{
	Component::Component(GameObject* owner) : m_owner(owner) {}
	Component::~Component() = default;

	void Component::Update(float deltaTime)		{ }
	void Component::LateUpdate(float deltaTime) { }

	GameObject* Component::GetOwner()
	{
		return m_owner;
	}

	const GameObject* Component::GetOwner() const
	{
		return m_owner;
	}

	Transform& Component::GetTransform()
	{
		MGSL_ASSERT(GetOwner() != nullptr);
		return GetOwner()->GetTransform();
	}

	const Transform& Component::GetTransform() const
	{
		MGSL_ASSERT(GetOwner() != nullptr);
		return GetOwner()->GetTransform();
	}
}