#pragma once
#include "GameObject.h"

namespace MGSL::Framework
{
    template<typename T>
    inline GameObject* ObjectRegistry::FindGameObjectWithComponent()
    {
        for (const GameObjectUPtr& gameObject : m_gameObjects)
        {
            if (!gameObject)  continue;
            if (gameObject->GetComponent<T>() != nullptr) return gameObject.get();
        }

        return nullptr;
    }
}