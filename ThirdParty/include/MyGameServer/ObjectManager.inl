#pragma once
#include "GameObject.h"

namespace MGSL::Server
{
	template<typename T, typename... Args>
	T* ObjectManager::AddComponent(const GameObjectPtr& gameObject, Args&&... args)
	{
		if (gameObject == nullptr) return nullptr;
		return gameObject->AddComponent<T>(std::forward<Args>(args)...);
	}
}