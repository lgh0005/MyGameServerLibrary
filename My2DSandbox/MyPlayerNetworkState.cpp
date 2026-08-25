#include "2DPch.h"
#include "MyPlayerNetworkState.h"

namespace MGSL::Sandbox2D
{
	MyPlayerNetworkState::MyPlayerNetworkState(Framework::GameObject* owner) : Super(owner) { }
	MyPlayerNetworkState::~MyPlayerNetworkState() = default;

	MyPlayerNetworkStateUPtr MyPlayerNetworkState::Create(Framework::GameObject* owner)
	{
		return MyPlayerNetworkStateUPtr(new MyPlayerNetworkState(owner));
	}

	void MyPlayerNetworkState::Apply(const ::Protobuf::PlayerInfo& info) { SetInfo(info); }

    ::Protobuf::OBJECT_STATE_TYPE MyPlayerNetworkState::GetState() const { return GetInfo().state(); }
    
	::Protobuf::FACING_TYPE MyPlayerNetworkState::GetFacing() const { return GetInfo().facing(); }
    
	::Protobuf::WEAPON_TYPE MyPlayerNetworkState::GetWeapon() const { return GetInfo().weapon(); }
	
	Shared::vec4 MyPlayerNetworkState::GetColor() const
	{
		const auto& color = GetInfo().color();
		return Shared::vec4{ color.r(), color.g(), color.b(), color.a() };
	}
}
