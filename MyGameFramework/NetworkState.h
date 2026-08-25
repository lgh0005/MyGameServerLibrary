#pragma once
#include "Component.h"

namespace MGSL::Framework
{
	template<typename TInfo>
	class NetworkState : public Component
	{
		MGSL_DISABLE_COPY(NetworkState)
		MGSL_DISABLE_MOVE(NetworkState)
		using Super = Component;

	public:
		virtual ~NetworkState() override;
		void SetInfo(const TInfo& info);
		void SetInfo(TInfo&& info);
		TInfo& GetInfo() noexcept;
		const TInfo& GetInfo() const noexcept;

	protected:
		explicit NetworkState(GameObject* owner);
		TInfo m_info;
	};
}

#include "NetworkState.inl"

