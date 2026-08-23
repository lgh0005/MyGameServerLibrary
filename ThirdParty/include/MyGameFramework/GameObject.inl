#pragma once

namespace MGSL::Framework
{
	template<typename T, typename... Args>
	inline T* GameObject::AddComponent(Args&&... args)
	{
		auto component = T::Create(this, std::forward<Args>(args)...);
		if (!component) return nullptr;

		T* componentPointer = component.get();
		m_components.push_back(std::move(component));

		return componentPointer;
	}

	template<typename T>
	T* GameObject::GetComponent()
	{
		for (const ComponentUPtr& component : m_components)
		{
			T* result = dynamic_cast<T*>(component.get());
			if (result != nullptr)
				return result;
		}

		return nullptr;
	}
}