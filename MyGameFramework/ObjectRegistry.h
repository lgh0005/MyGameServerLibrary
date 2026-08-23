#pragma once

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(GameObject)

	class ObjectRegistry
	{
		MGSL_DISABLE_COPY(ObjectRegistry)
		MGSL_DISABLE_MOVE(ObjectRegistry)

	public:
		ObjectRegistry();
		~ObjectRegistry();

	public:
		template<typename T> GameObject* FindGameObjectWithComponent();
		GameObject* Find(Shared::uint64 objectID);

	public:
		GameObject* Add(GameObjectUPtr go);
		void Remove(GameObject* go);
		const Shared::List<GameObject*>& Flush();
		void Clear();

	public:
		void Update(float deltaTime);
		void LateUpdate(float deltaTime);
		void Submit();

	private:
		bool IsPendingRemove(GameObject* go) const;
		void FlushRemoveObjects();
		void FlushAddObjects();

	private:
		Shared::List<GameObjectUPtr> m_gameObjects;
		Shared::List<GameObject*> m_addedObjects;
		Shared::List<GameObjectUPtr> m_pendingAddObjects;
		Shared::List<GameObject*> m_pendingRemoveObjects;
	};
}

#include "ObjectRegistry.inl"