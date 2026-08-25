#include "2DPch.h"
#include "BulletState.h"

namespace MGSL::Sandbox2D
{
	BulletState::BulletState(Framework::GameObject* owner) : Super(owner) { }
	BulletState::~BulletState() = default;

	BulletStateUPtr BulletState::Create(Framework::GameObject* owner)
	{
		return BulletStateUPtr(new BulletState(owner));
	}
}
