#pragma once
#include "MyGameFramework/NetworkState.h"

namespace MGSL::Sandbox2D
{
	MGSL_CLASS_PTR(MyPlayerNetworkState)

	class MyPlayerNetworkState final : public Framework::NetworkState<::Protobuf::PlayerInfo>
	{
		using Super = Framework::NetworkState<::Protobuf::PlayerInfo>;

	public:
		virtual ~MyPlayerNetworkState() override;
		static MyPlayerNetworkStateUPtr Create(Framework::GameObject* owner);

	public:
		void Apply(const ::Protobuf::PlayerInfo& info);
		::Protobuf::OBJECT_STATE_TYPE GetState() const;
		::Protobuf::FACING_TYPE GetFacing()		 const;
		::Protobuf::WEAPON_TYPE GetWeapon()		 const;
		float GetVerticalVelocity()				 const;
		Shared::vec4 GetColor()					 const;
		bool IsInvincible()						 const;
		Shared::uint32 GetLife()				 const;
		Shared::uint32 GetKillCount()			 const;

	private:
		explicit MyPlayerNetworkState(Framework::GameObject* owner);
	};
}

