#include "GameFrameworkPch.h"
#include "Resource.h"

namespace MGSL::Framework
{
	Resource::Resource(EResourceType type)
		: m_resourceType(type),
		  m_resourceState(EResourceState::UNLOADED) { }
	Resource::~Resource() = default;

	EResourceType Resource::GetResourceType() const
	{
		return m_resourceType;
	}

	EResourceState Resource::GetResourceState() const noexcept
	{
		return m_resourceState.load();
	}

	void Resource::SetResourceState(EResourceState state) noexcept
	{
		m_resourceState.store(state);
	}
}