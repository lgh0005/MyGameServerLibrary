#pragma once
#include "GameObject.h"

namespace MGSL::Framework
{
	template<typename T>
	GameObject* Scene::FindGameObjectWithComponent()
	{
		return m_objectRegistry.FindGameObjectWithComponent<T>();
	}
}