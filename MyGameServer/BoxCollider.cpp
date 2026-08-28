#include "ServerPch.h"
#include "BoxCollider.h"

namespace MGSL::Server
{
	/*=======================//
	//   Server BoxCollider  //
	//=======================*/
	BoxCollider::BoxCollider(GameObject* owner) : Super(owner) { }
	BoxCollider::~BoxCollider() = default;

	BoxColliderUPtr BoxCollider::Create(GameObject* owner)
	{
		return BoxColliderUPtr(new BoxCollider(owner));
	}

	void BoxCollider::OnDestroy()
	{
		MGSL_SERVER_COLLISION_MGR.Unregister(this);
	}

	void BoxCollider::SetSize(const Shared::vec2& size)
	{
		m_size = size;
	}

	void BoxCollider::SetOffset(const Shared::vec2& offset)
	{
		m_offset = offset;
	}

	void BoxCollider::SetMobility(EColliderMobility mobility)
	{
		m_mobility = mobility;
	}

	void BoxCollider::SetCollisionType(ECollisionType type)
	{
		m_collisoinType = type;
	}

	void BoxCollider::SetCollisionLayer(ECollisionLayer layer)
	{
		m_collisionLayer = layer;
	}

	void BoxCollider::SetTrigger(bool isTrigger)
	{
		m_isTrigger = isTrigger;
	}

	const Shared::vec2& BoxCollider::GetSize() const
	{
		return m_size;
	}

	const Shared::vec2& BoxCollider::GetOffset() const
	{
		return m_offset;
	}

	EColliderMobility BoxCollider::GetMobility() const
	{
		return m_mobility;
	}

	ECollisionType BoxCollider::GetCollisionType() const
	{
		return m_collisoinType;
	}

	ECollisionLayer BoxCollider::GetCollisionLayer() const
	{
		return m_collisionLayer;
	}

	bool BoxCollider::IsTrigger() const
	{
		return m_isTrigger;
	}

	const AABB& BoxCollider::GetBounds() const
	{
		return m_bounds;
	}

	void BoxCollider::UpdateBounds()
	{
		const Shared::vec3 position = GetTransform().GetWorldPosition();
		const Shared::vec3 scale = GetTransform().GetWorldScale();
		const Shared::vec2 center { position.x + m_offset.x, position.y + m_offset.y };
		const Shared::vec2 halfSize { m_size.x * scale.x * 0.5f, m_size.y * scale.y * 0.5f };
		m_bounds.min = center - halfSize;
		m_bounds.max = center + halfSize;
	}
}
