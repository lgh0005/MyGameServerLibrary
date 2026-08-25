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

	/*===================================//
	//   Network GameObject Management   //
	//===================================*/
	public:
		bool RegisterNetworkObject(Shared::uint64 objectID, GameObject* gameObject);
		GameObject* FindNetworkObject(Shared::uint64 objectID);
		bool UnregisterNetworkObject(Shared::uint64 objectID);

	/*==================================//
	//   Client GameObject Management   //
	//==================================*/
	public:
		template<typename T> GameObject* FindGameObjectWithComponent();
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
		Shared::Dictionary<Shared::uint64, GameObject*> m_networkObjects;
		Shared::List<GameObjectUPtr> m_gameObjects;
		Shared::List<GameObject*> m_addedObjects;
		Shared::List<GameObjectUPtr> m_pendingAddObjects;
		Shared::List<GameObject*> m_pendingRemoveObjects;
	};
}

#include "ObjectRegistry.inl"