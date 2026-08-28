#pragma once

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(Scene)
	MGSL_CLASS_PTR(GameObject)

	class ObjectManager
	{
		MGSL_DECLARE_SINGLE(ObjectManager)

	private:
		ObjectManager();
		~ObjectManager();

	/*============================//
	//   GameObject Creation      //
	//============================*/
	public:
		GameObject* CreateGameObject(Scene* scene);

	/*============================//
	//   GameObject Management    //
	//============================*/
	public:
		GameObject* AddGameObject(GameObjectUPtr go);
		void RemoveGameObject(Scene* scene, GameObject* go);
		template<typename T> GameObject* FindGameObjectWithComponent(Scene* scene);

		bool RegisterNetworkObject(Scene* scene, Shared::uint64 objectID, GameObject* gameObject);
		GameObject* FindNetworkObject(Scene* scene, Shared::uint64 objectID);
		bool UnregisterNetworkObject(Scene* scene, Shared::uint64 objectID);

	/*===========================//
	//   Component Creation      //
	//===========================*/
	public:
		template<typename T, typename... Args>
		T* AddComponent(GameObject* go, Args&&... args);
	};
}

#include "ObjectManager.inl"
