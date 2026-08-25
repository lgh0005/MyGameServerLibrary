#include "ServerPch.h"
#include "ObjectManager.h"
#include "GameObject.h"
#include "VirtualScene.h"

namespace MGSL::Server
{
    Shared::Atomic<Shared::uint64> ObjectManager::s_idGenerator = 0;

	ObjectManager::ObjectManager() = default;
	ObjectManager::~ObjectManager() = default;

    GameObject* ObjectManager::CreateGameObject(VirtualScene* scene)
    {
        if (!scene) return nullptr;

        GameObjectUPtr gameObject = GameObject::Create();
        if (!gameObject) return nullptr;
        
        return scene->AddGameObject(std::move(gameObject));
    }

    Shared::uint64 ObjectManager::GenerateNetworkObjectID()
    {
        return ++s_idGenerator;
    }

    void ObjectManager::RemoveGameObject(VirtualScene* scene, GameObject* gameObject)
    {
        if (!scene) return;
        if (!gameObject) return;
        scene->RemoveGameObject(gameObject);
    }
}
