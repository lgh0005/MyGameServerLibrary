#include "GameFrameworkPch.h"
#include "CollisionManager.h"
#include "BoxCollider.h"

namespace MGSL::Framework
{
	/*============================//
	//   Client CollisionManager  //
	//============================*/
	CollisionManager::CollisionManager() = default;
	CollisionManager::~CollisionManager() = default;

	void CollisionManager::Init()
	{
		m_colliders.reserve(128);
	}

	void CollisionManager::Clear()
	{
		m_colliders.clear();
	}

	void CollisionManager::Update(float deltaTime)
	{
		for (BoxCollider* collider : m_colliders)
		{
			if (!collider) continue;
			collider->Update(deltaTime);
		}
	}

	void CollisionManager::Register(BoxCollider* collider)
	{
		if (!collider) return;
		auto it = std::find(m_colliders.begin(), m_colliders.end(), collider);
		if (it != m_colliders.end()) return;
		m_colliders.push_back(collider);
	}

	void CollisionManager::Unregister(BoxCollider* collider)
	{
		if (!collider) return;
		auto it = std::find(m_colliders.begin(), m_colliders.end(), collider);
		if (it == m_colliders.end()) return;
		m_colliders.erase(it);
	}

	const Shared::List<BoxCollider*> CollisionManager::GetColliders() const
	{
		return m_colliders;
	}
}
