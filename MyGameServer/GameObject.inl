#pragma once

namespace MGSL::Server
{
    template<typename T, typename... Args>
    inline T* GameObject::AddComponent(Args&&... args)
    {
        auto component = T::Create(this, std::forward<Args>(args)...);
        if (!component) return nullptr;

        T* ptr = component.get();
        m_components.push_back(std::move(component));

        return ptr;
    }

    template<typename T>
    inline T* GameObject::GetComponent()
    {
        for (auto& component : m_components)
        {
            if (!component) continue;
            T* ptr = dynamic_cast<T*>(component.get());
            if (ptr) return ptr;
        }

        return nullptr;
    }
}