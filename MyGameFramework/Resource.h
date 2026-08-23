#pragma once

namespace MGSL::Framework
{
	class Resource
	{
	public:
		explicit Resource(EResourceType type = EResourceType::RESOURCE);
		virtual ~Resource();

	public:
		virtual bool Load() = 0;
		EResourceType GetResourceType() const;

	public:
		EResourceState GetResourceState() const noexcept;
		void SetResourceState(EResourceState state) noexcept;

	private:
		EResourceType m_resourceType;
		Shared::Atomic<EResourceState> m_resourceState;
	};
}


