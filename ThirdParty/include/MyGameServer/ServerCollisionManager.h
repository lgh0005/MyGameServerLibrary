#pragma once
#include "ColliderFunctor.h"
#include "CollisionResolveInfo.h"

namespace MGSL::Server
{
	MGSL_CLASS_PTR(BoxCollider)
	MGSL_CLASS_PTR(CharacterBody2D)
	MGSL_CLASS_PTR(ClientBehaviour)

	/*============================//
	//   Server CollisionManager  //
	//============================*/
	class ServerCollisionManager
	{
		MGSL_DECLARE_SINGLE(ServerCollisionManager)
		static constexpr Shared::usize LAYER_COUNT = static_cast<Shared::usize>(ECollisionLayer::COUNT);

	private:
		ServerCollisionManager();
		~ServerCollisionManager();

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

	private:
		CharacterBody2D* GetCharacterBody(BoxCollider* collider);
		bool GetCollisionResolveInfo(BoxCollider* lhs, BoxCollider* rhs, CollisionResolveInfo& outInfo);

	/*==============================================//
	//   collision event notifiers and resolvers    //
	//==============================================*/
	private:
		void ResolveCollisionEvents();
		void NotifyPair(const CollisionPair& pair, ECollisionEventState state);
		void NotifyBehaviour(ClientBehaviour* behaviour, BoxCollider* other,  ECollisionEventState state, bool isTrigger);

	private:
		void ResolveCollision(BoxCollider* lhs, BoxCollider* rhs);
		void ResolveStaticDynamic(BoxCollider* staticCollider, BoxCollider* dynamicCollider);
		void ResolveDynamicDynamic(BoxCollider* lhs, BoxCollider* rhs);

	private:
		void ResolveSolidStaticDynamic(BoxCollider* staticCollider, BoxCollider* dynamicCollider, CharacterBody2D* dynamicBody);
		void ResolvePlatformStaticDynamic(BoxCollider* staticCollider, BoxCollider* dynamicCollider, CharacterBody2D* dynamicBody);

	private:
		void ResolveBottomStaticDynamic(BoxCollider* staticCollider, BoxCollider* dynamicCollider, CharacterBody2D* dynamicBody);
		void ResolveTopStaticDynamic(BoxCollider* staticCollider, BoxCollider* dynamicCollider, CharacterBody2D* dynamicBody);
		void ResolveLeftStaticDynamic(BoxCollider* staticCollider, BoxCollider* dynamicCollider, CharacterBody2D* dynamicBody);
		void ResolveRightStaticDynamic(BoxCollider* staticCollider, BoxCollider* dynamicCollider, CharacterBody2D* dynamicBody);

	/*==============================//
	//   collision layer setters    //
	//==============================*/
	private:
		void SetLayerCollision(ECollisionLayer lhs, ECollisionLayer rhs, bool enable);
		bool CanCollide(ECollisionLayer lhs, ECollisionLayer rhs) const;

	private:
		bool m_collisionMatrix[LAYER_COUNT][LAYER_COUNT]{ };
		Shared::List<BoxCollider*> m_staticColliders;
		Shared::List<BoxCollider*> m_dynamicColliders;
		Shared::List<CollisionPair> m_previousCollisions;
		Shared::List<CollisionPair> m_currentCollisions;
	};
}

