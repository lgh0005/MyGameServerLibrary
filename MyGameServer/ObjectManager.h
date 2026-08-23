#pragma once

namespace MGSL::Server
{
	MGSL_CLASS_PTR(GameObject)

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
		GameObjectPtr CreateGameObject();
		void RemoveGameObject(const GameObjectPtr& go);
	
	public:
		template<typename T, typename... Args>
		T* AddComponent(const GameObjectPtr& go, Args&&... args);

	private:
		static Shared::Atomic<Shared::uint64> s_idGenerator;
	};
}

#include "ObjectManager.inl"