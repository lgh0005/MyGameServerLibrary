#pragma once
#include "Transform.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(Scene)
	MGSL_CLASS_PTR(Component)
	MGSL_CLASS_PTR(GameObject)

	/* clients's actual GameObject */
	class GameObject
	{
	public:
		virtual ~GameObject();
		static GameObjectUPtr Create(Scene* scene);
		Scene* GetOwner() const;

	/*==================================//
	//   default lifecycle interfaces   //
	//==================================*/
	public:
		void Awake();
		void Start();
		void Update(float deltaTime);
		void LateUpdate(float deltaTime);
		void Submit();

	/*===============================//
	//   component control methods   //
	//===============================*/
	public:
		template<typename T, typename... Args> T* AddComponent(Args&&... args);
		template<typename T> T* GetComponent();
		void RemoveComponent(Component* component);
		void ClearComponents();

	/*===============================//
	//   hierarchy control methods   //
	//===============================*/
	public:
		bool SetParent(GameObject* newParent);
		bool AddChild(GameObject* child);
		bool RemoveChild(GameObject* child);

		bool HasParent() const;
		GameObject* GetParent() const;
		const Shared::List<GameObject*>& GetChildren() const;

	private:
		bool IsAncestorOf(const GameObject* gameObject) const;
		void AddChildInternal(GameObject* child);
		void RemoveChildInternal(GameObject* child);
		void DetachHierarchy();

	/*===============================//
	//   default transform methods   //
	//===============================*/
	public:
		Transform& GetTransform();
		const Transform& GetTransform() const;

	/*======================================//
	//   default packet setter and getter   //
	//======================================*/
	public:
		void SetObjectInfo(const ::Protobuf::ObjectInfo& info);
		::Protobuf::ObjectInfo& GetObjectInfo();

	private:
		GameObject(Scene* scene);
		Scene* m_owner;

		Transform m_transform;
		Shared::List<ComponentUPtr> m_components;
	
		GameObject* m_parent = nullptr;
		Shared::List<GameObject*> m_children;

		// 패킷들
		::Protobuf::ObjectInfo m_info;
	};
}

#include "GameObject.inl"
