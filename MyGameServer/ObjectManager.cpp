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

        auto& info = gameObject->GetObjectInfo();
        info.set_objectid(++s_idGenerator);

        auto* color = info.mutable_color();
        color->set_r(1.0f);
        color->set_g(1.0f);
        color->set_b(1.0f);
        color->set_a(1.0f);
        
        return scene->AddGameObject(std::move(gameObject));
    }

    void ObjectManager::RemoveGameObject(VirtualScene* scene, GameObject* gameObject)
    {
        if (!scene) return;
        if (!gameObject) return;
        scene->RemoveGameObject(gameObject);
    }
}
