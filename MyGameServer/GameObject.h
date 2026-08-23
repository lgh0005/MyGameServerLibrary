#pragma once
#include "Transform.h"

namespace MGSL::Net 
{ 
	MGSL_CLASS_PTR(GameRoom)
	MGSL_CLASS_PTR(GameSession)
}

namespace MGSL::Server
{
	MGSL_CLASS_PTR(Component)
	MGSL_CLASS_PTR(GameObject)

	/* server's logical GameObject */
	class GameObject
	{
	public:
		virtual ~GameObject();
		static GameObjectPtr Create();

	public:
		void Update(float deltaTime);
		void LateUpdate(float deltaTime);

	public:
		template<typename T, typename... Args> T* AddComponent(Args&&... args);
		template<typename T> T* GetComponent();
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

		void SetGameRoom(Net::GameRoomPtr room) { m_gameRoom = room; }
		Net::GameRoomPtr GetGameRoom() const { return m_gameRoom.lock(); }
		void SetGameSession(Net::GameSessionPtr session) { m_gameSession = session; }
		Net::GameSessionPtr GetGameSession() const { return m_gameSession.lock(); }

	private:
		GameObject();

		::Protobuf::ObjectInfo m_info;
		Net::GameRoomWPtr m_gameRoom;
		Net::GameSessionWPtr m_gameSession;

		Transform m_transform;
		Shared::List<ComponentUPtr> m_components;
		GameObject* m_parent = nullptr;
		Shared::List<GameObject*> m_children;
	};
}

#include "GameObject.inl"