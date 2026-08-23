#include "GameFrameworkPch.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "Scene.h"

namespace MGSL::Framework
{
	ObjectManager::ObjectManager() = default;
	ObjectManager::~ObjectManager() = default;

	/*============================//
	//   GameObject Creation      //
	//============================*/
	GameObject* ObjectManager::CreateGameObject(Scene* scene)
	{
		GameObjectUPtr gameObject = GameObject::Create(scene);
		if (!gameObject) return nullptr;
		return scene->AddGameObject(std::move(gameObject));
	}

	/*============================//
	//   GameObject Management    //
	//============================*/
	GameObject* ObjectManager::AddGameObject(GameObjectUPtr gameObject)
	{
		if (!gameObject) return nullptr;

		Scene* scene = gameObject->GetOwner();
		if (scene == nullptr) return nullptr;
		return scene->AddGameObject(std::move(gameObject));
	}

	void ObjectManager::RemoveGameObject(GameObject* gameObject)
	{
		if (gameObject == nullptr) return;

		Scene* scene = gameObject->GetOwner();
		if (scene == nullptr) return;
		scene->RemoveGameObject(gameObject);
	}
}