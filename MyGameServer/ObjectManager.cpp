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
        GameObjectUPtr gameObject = GameObject::Create(scene);
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

        // 자식 먼저 제거
        const auto children = gameObject->GetChildren();
        for (GameObject* child : children)
        {
            if (!child) continue;
            RemoveGameObject(scene, child);
        }

        // 부모와 관계 제거
        GameObject* parent = gameObject->GetParent();
        if (parent) parent->RemoveChild(gameObject);

        // 실제 Scene에서 자기 자신 제거
        scene->RemoveGameObject(gameObject);
    }
}
