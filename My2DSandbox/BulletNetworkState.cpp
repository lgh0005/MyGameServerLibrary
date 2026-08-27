#include "2DPch.h"
#include "BulletNetworkState.h"
#include "MyGameFramework/SpriteRenderer.h"

namespace MGSL::Sandbox2D
{
	BulletNetworkState::BulletNetworkState(Framework::GameObject* owner) : Super(owner) { }
	BulletNetworkState::~BulletNetworkState() = default;

	BulletNetworkStateUPtr BulletNetworkState::Create(Framework::GameObject* owner)
	{
		return BulletNetworkStateUPtr(new BulletNetworkState(owner));
	}

	void BulletNetworkState::ApplyInfo(const ::Protobuf::BulletInfo& info)
	{
		SetInfo(info);

		Framework::GameObject* owner = GetOwner();
		if (!owner) return;

		// 이동 동기화
		owner->GetTransform().SetPosition(Shared::vec3(info.position().x(), info.position().y(), 0.0f));

		// 색상 유지
		Framework::SpriteRenderer* renderer = owner->GetComponent<Framework::SpriteRenderer>();
		if (renderer) renderer->SetColor(Shared::vec4(info.color().r(), info.color().g(), info.color().b(), info.color().a()));
	}

	Shared::uint64 BulletNetworkState::GetObjectID() const
	{
		return GetInfo().objectid();
	}

	Shared::uint64 BulletNetworkState::GetOwnerID() const
	{
		return GetInfo().ownerid();
	}
}
