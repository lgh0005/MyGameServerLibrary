#include "ServerPch.h"
#include "ObjectManager.h"
#include "GameObject.h"

namespace MGSL::Server
{
    Shared::Atomic<Shared::uint64> ObjectManager::s_idGenerator = 0;

	ObjectManager::ObjectManager() = default;
	ObjectManager::~ObjectManager() = default;

    GameObjectPtr ObjectManager::CreateGameObject()
    {
        auto gameObject = GameObject::Create();
        if (!gameObject) return nullptr;

        auto& info = gameObject->GetObjectInfo();
        info.set_objectid(++s_idGenerator);

        return gameObject;
    }

    void ObjectManager::RemoveGameObject(const GameObjectPtr& gameObject)
    {
        // TODO : GameRoom에서 해당 게임 오브젝트를 지워야 함
    }
}