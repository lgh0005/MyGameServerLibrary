#include "ServerPch.h"
#include "GameObject.h"
#include "Component.h"

namespace MGSL::Server
{
	GameObject::GameObject() = default;
	GameObject::~GameObject() = default;

	GameObjectUPtr GameObject::Create()
	{
		return GameObjectUPtr(new GameObject());
	}

	/*===================================//
	//   default lifecycle interfaces    //
	//===================================*/
	void GameObject::Update(float deltaTime) { for (const ComponentUPtr& component : m_components) component->Update(deltaTime); }
	void GameObject::LateUpdate(float deltaTime) { for (const ComponentUPtr& component : m_components) component->LateUpdate(deltaTime); }

	void GameObject::ClearComponents()
	{
		m_components.clear();
	}

	/*===============================//
	//   hierarchy control methods   //
	//===============================*/
	bool GameObject::SetParent(GameObject* newParent)
	{
		if (m_parent == newParent) return true;

		// 자기 자신 부모 지정 방지
		if (newParent == this) return false;

		// 순환 구조 방지
		if (IsAncestorOf(newParent)) return false;

		// 기존 GameObject 부모에서 자신 제거
		if (m_parent) m_parent->RemoveChildInternal(this);

		// 새 GameObject 부모에 자신 등록
		m_parent = newParent;
		if (m_parent) m_parent->AddChildInternal(this);

		// Transform 계층 동기화
		Transform* parentTransform = nullptr;
		if (m_parent) parentTransform = &m_parent->m_transform;
		m_transform.SetParentInternal(parentTransform);

		return true;
	}

	bool GameObject::AddChild(GameObject* child)
	{
		if (!child) return false;
		return child->SetParent(this);
	}

	bool GameObject::RemoveChild(GameObject* child)
	{
		if (!child) return false;
		if (child->m_parent != this) return false;
		return child->SetParent(nullptr);
	}

	bool GameObject::HasParent() const { return m_parent != nullptr; }
	GameObject* GameObject::GetParent() const { return m_parent; }
	const Shared::List<GameObject*>& GameObject::GetChildren() const { return m_children; }

	bool GameObject::IsAncestorOf(const GameObject* gameObject) const
	{
		if (!gameObject) return false;

		const GameObject* current = gameObject->m_parent;
		while (current)
		{
			if (current == this) return true;
			current = current->m_parent;
		}

		return false;
	}

	void GameObject::AddChildInternal(GameObject* child)
	{
		if (!child) return;
		const auto it = std::find
		(
			m_children.begin(),
			m_children.end(),
			child
		);
		if (it != m_children.end()) return;

		m_children.push_back(child);
	}

	void GameObject::RemoveChildInternal(GameObject* child)
	{
		if (!child) return;
		const auto it = std::find
		(
			m_children.begin(),
			m_children.end(),
			child
		);
		if (it == m_children.end()) return;

		m_children.erase(it);
	}

	void GameObject::DetachHierarchy()
	{
		// 기존 부모로부터 자신을 먼저 분리
		SetParent(nullptr);

		// 각 GameObject가 가지는 부모 연결을 제거
		const Shared::List<GameObject*> children = m_children;
		for (GameObject* child : children)
		{
			if (child)
				child->SetParent(nullptr);
		}

		// 자식들 비우기
		m_children.clear();
	}
	/*===============================//
	//   default transform methods   //
	//===============================*/
	Transform& GameObject::GetTransform() { return m_transform; }

	const Transform& GameObject::GetTransform() const { return m_transform; }
}
