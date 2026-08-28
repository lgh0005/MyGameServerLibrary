#include "GameFrameworkPch.h"
#include "Component.h"
#include "GameObject.h"

namespace MGSL::Framework
{
	Component::Component(GameObject* owner) : m_owner(owner) { }
	Component::~Component() = default;

	void Component::Awake()						{ }
	void Component::Start()						{ }
	void Component::Update(float deltaTime)		{ }
	void Component::LateUpdate(float deltaTime) { }
	void Component::Submit()					{ }
	void Component::OnDestroy()					{ }

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