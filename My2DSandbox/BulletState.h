#pragma once
#include "MyGameFramework/NetworkState.h"

namespace MGSL::Sandbox2D
{
	MGSL_CLASS_PTR(BulletState)

	class BulletState final : public Framework::NetworkState<::Protobuf::BulletInfo>
	{
		MGSL_DISABLE_COPY(BulletState)
		MGSL_DISABLE_MOVE(BulletState)
		using Super = Framework::NetworkState<::Protobuf::BulletInfo>;

	public:
		virtual ~BulletState() override;
		static BulletStateUPtr Create(Framework::GameObject* owner);

	private:
		explicit BulletState(Framework::GameObject* owner);
	};
}

