#pragma once
#include "MyGameFramework/NetworkState.h"

namespace MGSL::Sandbox2D
{
	MGSL_CLASS_PTR(BulletNetworkState)

	class BulletNetworkState final : public Framework::NetworkState<::Protobuf::BulletInfo>
	{
		MGSL_DISABLE_COPY(BulletNetworkState)
		MGSL_DISABLE_MOVE(BulletNetworkState)
		using Super = Framework::NetworkState<::Protobuf::BulletInfo>;

	public:
		virtual ~BulletNetworkState() override;
		static BulletNetworkStateUPtr Create(Framework::GameObject* owner);

	public:
		void ApplyInfo(const ::Protobuf::BulletInfo& info);
		Shared::uint64 GetObjectID() const;
		Shared::uint64 GetOwnerID() const;

	private:
		explicit BulletNetworkState(Framework::GameObject* owner);
	};
}

