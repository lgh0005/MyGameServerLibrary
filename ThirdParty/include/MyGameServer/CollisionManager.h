#pragma once
#include "ColliderFunctor.h"

namespace MGSL::Server
{
	MGSL_CLASS_PTR(BoxCollider)
	MGSL_CLASS_PTR(ClientBehaviour)

	class CollisionManager
	{
		MGSL_DECLARE_SINGLE(CollisionManager)

	private:
		CollisionManager();
		~CollisionManager();

	public:
		void Init();
		void Update();
		void Clear();

	public:
		const Shared::List<BoxCollider*>& GetStaticColliders() const;
		const Shared::List<BoxCollider*>& GetDynamicColliders() const;

	public:
		void Register(BoxCollider* collider);
		void Unregister(BoxCollider* collider);

	/*===========================//
	//   collision managements   //
	//===========================*/
	private:
		void UpdateDynamicBounds();
		void UpdateDynamicSortOrder();

		void BroadPhase();
		void SweepDynamicStatic();
		void SweepDynamicDynamic();

	/*==============================//
	//   collision event handlers   //
	//==============================*/
	private:
		void ResolveCollisionEvents();
		void NotifyPair(const CollisionPair& pair, ECollisionEventState state);
		void NotifyBehaviour
		(
			ClientBehaviour* behaviour,
			BoxCollider* other, 
			ECollisionEventState state, 
			bool isTrigger
		);

	private:
		Shared::List<BoxCollider*> m_staticColliders;
		Shared::List<BoxCollider*> m_dynamicColliders;
		Shared::List<CollisionPair> m_previousCollisions;
		Shared::List<CollisionPair> m_currentCollisions;
	};
}

