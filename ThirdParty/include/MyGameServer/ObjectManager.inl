#pragma once
#include "GameObject.h"

namespace MGSL::Server
{
	template<typename T, typename... Args>
	T* ObjectManager::AddComponent(GameObject* go, Args&&... args)
	{
		if (go == nullptr) return nullptr;
		return go->AddComponent<T>(std::forward<Args>(args)...);
	}
}