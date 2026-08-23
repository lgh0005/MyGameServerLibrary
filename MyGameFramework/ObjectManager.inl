#pragma once
#include "Scene.h"
#include "GameObject.h"

namespace MGSL::Framework
{
	template<typename T, typename... Args>
	inline T* ObjectManager::AddComponent(GameObject* gameObject, Args&&... args)
	{
		if (gameObject == nullptr) return nullptr;
		return gameObject->AddComponent<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	inline GameObject* ObjectManager::FindGameObjectWithComponent(Scene* scene)
	{
		if (!scene) return nullptr;
		return scene->FindGameObjectWithComponent<T>();
	}
}