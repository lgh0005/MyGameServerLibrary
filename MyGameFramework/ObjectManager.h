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
		void RemoveGameObject(GameObject* go);
		GameObject* FindGameObject(Scene* scene, Shared::uint64 objectID);

		template<typename T>
		GameObject* FindGameObjectWithComponent(Scene* scene);

	/*===========================//
	//   Component Creation      //
	//===========================*/
	public:
		template<typename T, typename... Args>
		T* AddComponent(GameObject* go, Args&&... args);
	};
}

#include "ObjectManager.inl"
