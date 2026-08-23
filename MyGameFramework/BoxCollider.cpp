#include "GameFrameworkPch.h"
#include "BoxCollider.h"

namespace MGSL::Framework
{
	/*=======================//
	//   Client BoxCollider  //
	//=======================*/
	BoxCollider::BoxCollider(GameObject* owner) : Super(owner) { }
	BoxCollider::~BoxCollider() = default;

	BoxColliderUPtr BoxCollider::Create(GameObject* owner)
	{
		return BoxColliderUPtr(new BoxCollider(owner));
	}

	void BoxCollider::Update(float deltaTime)
	{
		if (!m_serverPosition.has_value()) return;

		const Shared::vec3& serverPosition = m_serverPosition.value();
		const Shared::vec3 localPosition = GetTransform().GetPosition();
		const Shared::vec3 difference = serverPosition - localPosition;
		const float distance = glm::length(difference);

		if (distance < m_ignoreThreshold) return;

		if (distance > m_snapThreshold)
		{
			GetTransform().SetPosition(serverPosition);
			return;
		}

		const float t = glm::clamp(m_correctionSpeed * deltaTime, 0.0f, 1.0f);
		const Shared::vec3 correctedPosition = glm::mix(localPosition, serverPosition, t);
		GetTransform().SetPosition(correctedPosition);
	}

	void BoxCollider::SetServerPosition(const Shared::vec3& position)
	{
		m_serverPosition = position;
	}

	void BoxCollider::SetTrigger(bool isTrigger)
	{
		m_isTrigger = isTrigger;
	}

	bool BoxCollider::IsTrigger() const
	{
		return m_isTrigger;
	}

	void BoxCollider::SetDebugSize(const Shared::vec2& size)
	{
		m_debugSize = size;
	}

	void BoxCollider::SetDebugOffset(const Shared::vec2& offset)
	{
		m_debugOffset = offset;
	}

	DebugGizmo BoxCollider::GetDebugGizmo() const
	{
		const Shared::vec3 position = GetTransform().GetPosition();
		const Shared::vec3 scale = GetTransform().GetScale();
		const Shared::vec2 center(position.x + m_debugOffset.x, position.y + m_debugOffset.y);
		const Shared::vec2 halfSize(m_debugSize.x * scale.x * 0.5f, m_debugSize.y * scale.y * 0.5f);

		DebugGizmo gizmo;
		gizmo.min = center - halfSize;
		gizmo.max = center + halfSize;
		return gizmo;
	}
}


