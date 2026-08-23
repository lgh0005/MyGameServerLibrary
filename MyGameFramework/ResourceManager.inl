#pragma once
#include "Resource.h"

namespace MGSL::Framework
{
    template<typename T>
    inline Shared::SharedPtr<T> ResourceManager::GetResource(Shared::String name)
    {
        const auto iter = m_resources.find(name);
        if (iter == m_resources.end()) return nullptr;

        const ResourcePtr& resource = iter->second;
        if (resource->GetResourceState() != EResourceState::READY)
            return nullptr;

        return std::static_pointer_cast<T>(resource);
    }

    template<typename T, typename... Args>
    inline bool ResourceManager::RegisterResource
    (
        Shared::String name,
        Args&&... args
    )
    {
        // 이미 생성된 리소스면 생략
        if (m_resources.contains(name))
            return false;

        // 리소스 인스턴스 생성
        Shared::SharedPtr<T> resource = std::make_shared<T>(std::forward<Args>(args)...);
        if (!resource) return false;

        // 리소스 인스턴스를 m_resources에 캐싱
        const auto [iter, inserted] = m_resources.emplace(name, resource);

        // RequestLoad로 로드 시작
        if (!RequestLoad(resource))
        {
            m_resources.erase(iter);
            return false;
        }

        return true;
    }
}
