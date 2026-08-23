#pragma once

namespace MGSL::Server
{
	class Transform
	{
		MGSL_DISABLE_COPY(Transform)
		MGSL_DISABLE_MOVE(Transform)
		friend class GameObject;

	public:
		Transform();
		~Transform();

	/*=========================================//
	//   local transform setters and getters   //
	//=========================================*/
	public:
		void SetPosition(const Shared::vec3& position);
		void SetRotation(const Shared::vec3& rotation);
		void SetScale(const Shared::vec3& scale);

		void Translate(const Shared::vec3& translation);
		void Rotate(const Shared::vec3& rotation);
		void Scale(const Shared::vec3& scale);

		const Shared::vec3& GetPosition()	 const;
		Shared::vec3 GetRotation()			 const;
		const Shared::vec3& GetScale()		 const;

		const Shared::quat& GetQuaternion()  const;
		const Shared::mat4& GetLocalMatrix() const;

	/*===============================//
	//   world transform getters     //
	//===============================*/
	public:
		Shared::vec3 GetWorldPosition()		 const;
		Shared::vec3 GetWorldRotation()		 const;
		Shared::vec3 GetWorldScale()		 const;

		Shared::quat GetWorldQuaternion()	 const;
		const Shared::mat4& GetWorldMatrix() const;

	/*===============================//
	//   hierarchy query methods     //
	//===============================*/
	public:
		bool HasParent() const;
		const Transform* GetParent() const;
		const Shared::List<Transform*>& GetChildren() const;

	private:
		void SetParentInternal(Transform* newParent);
		void AddChildInternal(Transform* child);
		void RemoveChildInternal(Transform* child);
		bool IsAncestorOf(const Transform* transform) const;

	/*=====================================//
	//   transform matrix update methods   //
	//=====================================*/
	private:
		void UpdateLocalMatrix()	 const;
		void UpdateWorldMatrix()	 const;
		void SetLocalDirty();
		void SetWorldDirty();

	private:
		Shared::vec3 m_position	{ 0.0f };
		Shared::quat m_rotation	{ 1.0f, 0.0f, 0.0f, 0.0f };
		Shared::vec3 m_scale	{ 1.0f };

		mutable Shared::mat4 m_localMatrix{ 1.0f };
		mutable Shared::mat4 m_worldMatrix{ 1.0f };

		mutable bool m_isLocalDirty = true;
		mutable bool m_isWorldDirty = true;

		Transform* m_parent = nullptr;
		Shared::List<Transform*> m_children;
	};
}

