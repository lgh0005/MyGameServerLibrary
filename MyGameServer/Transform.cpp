#include "ServerPch.h"
#include "Transform.h"

namespace MGSL::Server
{
	Transform::Transform() = default;
	Transform::~Transform()
	{
		m_parent = nullptr;
		m_children.clear();
	}

	/*=========================================//
	//   local transform setters and getters   //
	//=========================================*/
	void Transform::SetPosition(const Shared::vec3& position)  { m_position = position; SetLocalDirty(); }
	void Transform::SetRotation(const Shared::vec3& rotation)  { m_rotation = glm::normalize(Shared::quat(glm::radians(rotation))); SetLocalDirty(); }
	void Transform::SetScale(const Shared::vec3& scale)		   { m_scale = scale; SetLocalDirty(); }
	void Transform::Translate(const Shared::vec3& translation) { m_position += translation; SetLocalDirty(); }
	void Transform::Scale(const Shared::vec3& scale)		   { m_scale *= scale; SetLocalDirty(); }
	void Transform::Rotate(const Shared::vec3& rotation)
	{
		const Shared::quat deltaRotation(glm::radians(rotation));
		m_rotation = glm::normalize(m_rotation * deltaRotation);
		SetLocalDirty();
	}

	const Shared::vec3& Transform::GetPosition() const	  { return m_position; }
	Shared::vec3 Transform::GetRotation() const			  { return glm::degrees(glm::eulerAngles(m_rotation)); }
	const Shared::vec3& Transform::GetScale() const		  { return m_scale; }
	const Shared::quat& Transform::GetQuaternion() const  { return m_rotation; }
	const Shared::mat4& Transform::GetLocalMatrix() const { if (m_isLocalDirty) UpdateLocalMatrix(); return m_localMatrix; }
	const Shared::mat4& Transform::GetWorldMatrix() const { if (m_isWorldDirty) UpdateWorldMatrix(); return m_worldMatrix; }

	/*===============================//
	//   world transform getters     //
	//===============================*/
	Shared::vec3 Transform::GetWorldPosition() const { return Shared::vec3(GetWorldMatrix()[3]); }
	Shared::vec3 Transform::GetWorldRotation() const { return glm::degrees(glm::eulerAngles(GetWorldQuaternion())); }
	Shared::vec3 Transform::GetWorldScale() const
	{
		if (!m_parent) return m_scale;
		return m_parent->GetWorldScale() * m_scale;
	}

	Shared::quat Transform::GetWorldQuaternion() const
	{
		if (!m_parent) return m_rotation;
		return glm::normalize(m_parent->GetWorldQuaternion() * m_rotation);
	}

	/*===============================//
	//   hierarchy query methods     //
	//===============================*/
	bool Transform::HasParent() const { return m_parent != nullptr; }
	const Transform* Transform::GetParent() const { return m_parent; }
	const Shared::List<Transform*>& Transform::GetChildren() const { return m_children; }

	void Transform::SetParentInternal(Transform* newParent)
	{
		if (m_parent == newParent) return;
		if (m_parent) m_parent->RemoveChildInternal(this);

		m_parent = newParent;
		if (m_parent) m_parent->AddChildInternal(this);

		SetWorldDirty();
	}

	void Transform::AddChildInternal(Transform* child)
	{
		if (!child) return;

		const auto it = std::find
		(
			m_children.begin(),
			m_children.end(),
			child
		);
		if (it != m_children.end()) return;

		m_children.push_back(child);
	}

	void Transform::RemoveChildInternal(Transform* child)
	{
		if (!child) return;

		const auto it = std::find
		(
			m_children.begin(),
			m_children.end(),
			child
		);
		if (it == m_children.end()) return;

		m_children.erase(it);
	}

	bool Transform::IsAncestorOf(const Transform* transform) const
	{
		if (!transform) return false;

		const Transform* current = transform->m_parent;
		while (current)
		{
			if (current == this) return true;
			current = current->m_parent;
		}

		return false;
	}

	/*=====================================//
	//   transform matrix update methods   //
	//=====================================*/
	void Transform::UpdateLocalMatrix() const
	{
		const Shared::mat4 translationMatrix = glm::translate(Shared::mat4(1.0f), m_position);
		const Shared::mat4 rotationMatrix = glm::mat4_cast(m_rotation);
		const Shared::mat4 scaleMatrix = glm::scale(Shared::mat4(1.0f), m_scale);

		m_localMatrix = translationMatrix * rotationMatrix * scaleMatrix;
		m_isLocalDirty = false;
	}

	void Transform::UpdateWorldMatrix() const
	{
		if (m_parent) { m_worldMatrix = m_parent->GetWorldMatrix() * GetLocalMatrix(); }
		else { m_worldMatrix = GetLocalMatrix(); }
		m_isWorldDirty = false;
	}

	void Transform::SetLocalDirty() { m_isLocalDirty = true; SetWorldDirty(); }

	void Transform::SetWorldDirty()
	{
		m_isWorldDirty = true;
		for (Transform* child : m_children) 
		{ 
			if (child) 
				child->SetWorldDirty(); 
		}
	}
}


