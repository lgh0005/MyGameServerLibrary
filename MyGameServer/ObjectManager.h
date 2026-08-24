#pragma once

namespace MGSL::Server
{
	MGSL_CLASS_PTR(GameObject)
	MGSL_CLASS_PTR(VirtualScene)

	class ObjectManager
	{
		MGSL_DECLARE_SINGLE(ObjectManager)

	private:
		ObjectManager();
		~ObjectManager();

	/*============================//
	//   GameObject Management    //
	//============================*/
	public:
		GameObject* CreateGameObject(VirtualScene* scene);
		void RemoveGameObject(VirtualScene* scene, GameObject* gameObject);
	
	public:
		template<typename T, typename... Args>
		T* AddComponent(GameObject* go, Args&&... args);

	private:
		static Shared::Atomic<Shared::uint64> s_idGenerator;
	};
}

#include "ObjectManager.inl"