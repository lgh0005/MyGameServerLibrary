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

	bool ObjectManager::RegisterNetworkObject(Scene* scene, Shared::uint64 objectID, GameObject* gameObject)
	{
		if (!scene || !gameObject) return false;
		return scene->RegisterNetworkObject(objectID, gameObject);
	}

	GameObject* ObjectManager::FindNetworkObject(Scene* scene, Shared::uint64 objectID)
	{
		if (!scene) return nullptr;
		return scene->FindNetworkObject(objectID);
	}

	bool ObjectManager::UnregisterNetworkObject(Scene* scene, Shared::uint64 objectID)
	{
		if (!scene) return false;
		return scene->UnregisterNetworkObject(objectID);
	}
}
