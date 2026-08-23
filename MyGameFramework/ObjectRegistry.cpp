#include "GameFrameworkPch.h"
#include "ObjectRegistry.h"

namespace MGSL::Framework
{
	ObjectRegistry::ObjectRegistry() = default;
	ObjectRegistry::~ObjectRegistry() { Clear(); }

	GameObject* ObjectRegistry::Find(Shared::uint64 objectID)
	{
		for (const GameObjectUPtr& gameObject : m_gameObjects)
		{
			if (!gameObject) continue;
			if (gameObject->GetObjectInfo().objectid() == objectID)
				return gameObject.get();
		}

		return nullptr;
	}

	GameObject* ObjectRegistry::Add(GameObjectUPtr go)
	{
		if (!go) return nullptr;
		GameObject* obj = go.get();
		m_pendingAddObjects.push_back(std::move(go));
		return obj;
	}

	void ObjectRegistry::Remove(GameObject* go)
	{
		if (!go) return;
		if (IsPendingRemove(go)) return;
		m_pendingRemoveObjects.push_back(go);
	}

	const Shared::List<GameObject*>& ObjectRegistry::Flush()
	{
		m_addedObjects.clear();
		FlushRemoveObjects();
		FlushAddObjects();
		return m_addedObjects;
	}

	void ObjectRegistry::Clear()
	{
		m_pendingRemoveObjects.clear();
		m_pendingAddObjects.clear();
		m_gameObjects.clear();
	}

	void ObjectRegistry::Update(float deltaTime)
	{
		for (const GameObjectUPtr& gameObject : m_gameObjects)
		{
			if (gameObject)
				gameObject->Update(deltaTime);
		}
	}

	void ObjectRegistry::LateUpdate(float deltaTime)
	{
		for (const GameObjectUPtr& gameObject : m_gameObjects)
		{
			if (gameObject)
				gameObject->LateUpdate(deltaTime);
		}
	}

	void ObjectRegistry::Submit()
	{
		for (const GameObjectUPtr& gameObject : m_gameObjects)
		{
			if (gameObject)
				gameObject->Submit();
		}
	}

	bool ObjectRegistry::IsPendingRemove(GameObject* go) const
	{
		return std::find
		(
			m_pendingRemoveObjects.begin(),
			m_pendingRemoveObjects.end(),
			go
		) != m_pendingRemoveObjects.end();
	}

	void ObjectRegistry::FlushRemoveObjects()
	{
		if (m_pendingRemoveObjects.empty()) return;

		// 아직 활성 목록에 들어가지 않은 객체 중에서 같은 프레임에 삭제 요청까지 받은 객체를 제거
		m_pendingAddObjects.erase
		(
			std::remove_if
			(
				m_pendingAddObjects.begin(), m_pendingAddObjects.end(),
				[this](const GameObjectUPtr& gameObject) { return IsPendingRemove( gameObject.get() ); }
			),
			m_pendingAddObjects.end()
		);

		// 현재 활성 상태인 객체들 중에서 삭제 요청을 받은 객체를 제거
		m_gameObjects.erase
		(
			std::remove_if
			(
				m_gameObjects.begin(), m_gameObjects.end(),
				[this](const GameObjectUPtr& gameObject) { return IsPendingRemove(gameObject.get()); }
			),
			m_gameObjects.end()
		);

		// 모든 삭제 요청을 처리했으므로 대기 목록을 비운다.
		m_pendingRemoveObjects.clear();
	}

	void ObjectRegistry::FlushAddObjects()
	{
		// 추가 대기 중인 객체가 없다면 종료한다.
		if (m_pendingAddObjects.empty()) return;

		// 새로 활성화된 객체를 기록하는 과정에서
		// 불필요한 재할당이 발생하지 않도록 용량을 확보
		m_addedObjects.reserve(m_pendingAddObjects.size());
		m_gameObjects.reserve(m_gameObjects.size() + m_pendingAddObjects.size());

		// 추가 대기 객체를 활성 객체 목록으로 이전
		for (GameObjectUPtr& gameObject : m_pendingAddObjects)
		{
			if (!gameObject) continue;
			GameObject* obj = gameObject.get();
			m_gameObjects.push_back(std::move(gameObject));
			m_addedObjects.push_back(obj);
		}

		m_pendingAddObjects.clear();
	}
}