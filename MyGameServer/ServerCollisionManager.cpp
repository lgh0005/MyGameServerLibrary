#include "ServerPch.h"
#include "ServerCollisionManager.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "CharacterBody2D.h"
#include "ClientBehaviour.h"

namespace MGSL::Server
{
	/*============================//
	//   Server CollisionManager  //
	//============================*/
	ServerCollisionManager::ServerCollisionManager() = default;
	ServerCollisionManager::~ServerCollisionManager() = default;

	void ServerCollisionManager::Init()
	{
		m_staticColliders.reserve(128);
		m_dynamicColliders.reserve(128);
	}

	void ServerCollisionManager::Clear()
	{
		m_staticColliders.clear();
		m_dynamicColliders.clear();
		m_previousCollisions.clear();
		m_currentCollisions.clear();
	}

	void ServerCollisionManager::Update()
	{
		m_currentCollisions.clear();

		// Dynamic Collider는 움직일 수 있으므로 매 프레임 AABB를 갱신
		// 이전 프레임의 정렬 상태를 최대한 이용해 min.x 기준 정렬 순서를 보정
		UpdateDynamicBounds();
		UpdateDynamicSortOrder();

		// 실제 Broad Phase
		BroadPhase();

		// 이벤트 처리
		ResolveCollisionEvents();
		std::swap(m_previousCollisions, m_currentCollisions);
	}

	const Shared::List<BoxCollider*>& ServerCollisionManager::GetStaticColliders() const
	{
		return m_staticColliders;
	}

	const Shared::List<BoxCollider*>& ServerCollisionManager::GetDynamicColliders() const
	{
		return m_dynamicColliders;
	}

	void ServerCollisionManager::Register(BoxCollider* collider)
	{
		if (!collider) return;
		collider->UpdateBounds();

		switch (collider->GetMobility())
		{
			case EColliderMobility::STATIC:
			{
				auto it = std::lower_bound(m_staticColliders.begin(), m_staticColliders.end(), collider, BoxColliderLess{});
				m_staticColliders.insert(it, collider);
				break;
			}

			case EColliderMobility::DYNAMIC:
			{
				m_dynamicColliders.push_back(collider);
				break;
			}
		}
	}

	void ServerCollisionManager::Unregister(BoxCollider* collider)
	{
		if (!collider) return;

		switch (collider->GetMobility())
		{
			case EColliderMobility::STATIC:
			{
				auto it = std::find(m_staticColliders.begin(), m_staticColliders.end(), collider);
				if (it != m_staticColliders.end()) m_staticColliders.erase(it);
				break;
			}

			case EColliderMobility::DYNAMIC:
			{
				auto it = std::find(m_dynamicColliders.begin(), m_dynamicColliders.end(), collider);
				if (it != m_dynamicColliders.end()) m_dynamicColliders.erase(it);
				break;
			}
		}

		std::erase_if(m_previousCollisions, [collider](const CollisionPair& pair) { return pair.first == collider || pair.second == collider; });
		std::erase_if(m_currentCollisions, [collider](const CollisionPair& pair) { return pair.first == collider || pair.second == collider; });
	}

	/*===========================//
	//   collision managements   //
	//===========================*/
	void ServerCollisionManager::BroadPhase()
	{
		SweepDynamicStatic();
		SweepDynamicDynamic();
	}

	void ServerCollisionManager::UpdateDynamicBounds()
	{
		for (BoxCollider* collider : m_dynamicColliders)
		{
			if (!collider) continue;
			collider->UpdateBounds();
		}
	}

	void ServerCollisionManager::UpdateDynamicSortOrder()
	{
		// 정렬 순서가 크게 변하지 않으므로 삽입 정렬
		for (Shared::usize i = 1; i < m_dynamicColliders.size(); ++i)
		{
			BoxCollider* current = m_dynamicColliders[i];
			if (!current) continue;

			const float currentMinX = current->GetBounds().min.x;
			Shared::usize j = i;
			while (j > 0)
			{
				BoxCollider* previous = m_dynamicColliders[j - 1];
				if (!previous) break;
				if (previous->GetBounds().min.x <= currentMinX) break;
				m_dynamicColliders[j] = m_dynamicColliders[j - 1];
				--j;
			}

			m_dynamicColliders[j] = current;
		}
	}

	void ServerCollisionManager::SweepDynamicStatic()
	{
		if (m_dynamicColliders.empty() || m_staticColliders.empty())
			return;

		for (BoxCollider* dynamicCollider : m_dynamicColliders)
		{
			if (!dynamicCollider) continue;
			const AABB& dynamicBounds = dynamicCollider->GetBounds();

			for (BoxCollider* staticCollider : m_staticColliders)
			{
				if (!staticCollider) continue;
				const AABB& staticBounds = staticCollider->GetBounds();

				if (staticBounds.min.x > dynamicBounds.max.x) break;
				if (staticBounds.max.x < dynamicBounds.min.x) continue;
				if (dynamicBounds.Intersects(staticBounds))
					m_currentCollisions.emplace_back(dynamicCollider, staticCollider);
			}
		}
	}

	void ServerCollisionManager::SweepDynamicDynamic()
	{
		const Shared::usize count = m_dynamicColliders.size();
		if (count < 2) return;

		for (Shared::usize i = 0; i < count; ++i)
		{
			BoxCollider* lhs = m_dynamicColliders[i];
			if (!lhs) continue;

			const AABB& lhsBounds = lhs->GetBounds();
			for (Shared::usize j = i + 1; j < count; ++j)
			{
				BoxCollider* rhs = m_dynamicColliders[j];
				if (!rhs) continue;

				const AABB& rhsBounds = rhs->GetBounds();
				if (rhsBounds.min.x > lhsBounds.max.x) break;
				if (lhsBounds.Intersects(rhsBounds))
					m_currentCollisions.emplace_back(lhs, rhs);
			}
		}
	}

	CharacterBody2D* ServerCollisionManager::GetCharacterBody(BoxCollider* collider)
	{
		if (!collider) return nullptr;
		GameObject* owner = collider->GetOwner();
		if (!owner) return nullptr;
		return owner->GetComponent<CharacterBody2D>();
	}

	bool ServerCollisionManager::GetCollisionResolveInfo(BoxCollider* lhs, BoxCollider* rhs, CollisionResolveInfo& outInfo)
	{
		if (!lhs || !rhs) return false;

		outInfo.lhsBounds = lhs->GetBounds();
		outInfo.rhsBounds = rhs->GetBounds();
		outInfo.intersection = outInfo.lhsBounds & outInfo.rhsBounds;
		if (!outInfo.intersection.IsValid()) return false;

		outInfo.intersectionSize = outInfo.intersection.GetSize();
		outInfo.lhsCenter = outInfo.lhsBounds.GetCenter();
		outInfo.rhsCenter = outInfo.rhsBounds.GetCenter();

		return true;
	}

	/*===============================//
	//   collision event resolvers   //
	//===============================*/
	void ServerCollisionManager::ResolveCollisionEvents()
	{
		// Enter / Stay
		for (const CollisionPair& current : m_currentCollisions)
		{
			BoxCollider* lhs = current.first;
			BoxCollider* rhs = current.second;
			if (!lhs || !rhs) continue;

			const bool isTrigger = lhs->IsTrigger() || rhs->IsTrigger();
			if (!isTrigger) ResolveCollision(lhs, rhs);

			auto it = std::find(m_previousCollisions.begin(), m_previousCollisions.end(), current);
			if (it == m_previousCollisions.end()) NotifyPair(current, ECollisionEventState::ENTER);
			else NotifyPair(current, ECollisionEventState::STAY);
		}

		// Exit
		for (const CollisionPair& previous : m_previousCollisions)
		{
			auto it = std::find(m_currentCollisions.begin(), m_currentCollisions.end(), previous);
			if (it == m_currentCollisions.end()) NotifyPair(previous, ECollisionEventState::EXIT);
		}
	}

	void ServerCollisionManager::NotifyPair(const CollisionPair& pair, ECollisionEventState state)
	{
		BoxCollider* lhs = pair.first;
		BoxCollider* rhs = pair.second;
		if (!lhs || !rhs) return;

		const bool isTrigger = lhs->IsTrigger() || rhs->IsTrigger();

		GameObject* lhsOwner = lhs->GetOwner();
		GameObject* rhsOwner = rhs->GetOwner();

		if (lhsOwner)
		{
			ClientBehaviour* behaviour = lhsOwner->GetComponent<ClientBehaviour>();
			if (behaviour) NotifyBehaviour(behaviour, rhs, state, isTrigger);
		}

		if (rhsOwner)
		{
			ClientBehaviour* behaviour = rhsOwner->GetComponent<ClientBehaviour>();
			if (behaviour) NotifyBehaviour(behaviour, lhs, state, isTrigger);
		}
	}

	void ServerCollisionManager::NotifyBehaviour(ClientBehaviour* behaviour, BoxCollider* other, ECollisionEventState state, bool isTrigger)
	{
		if (!behaviour || !other) return;

		if (isTrigger)
		{
			switch (state)
			{
				case ECollisionEventState::ENTER: behaviour->OnTriggerEnter(other); break;
				case ECollisionEventState::STAY: behaviour->OnTriggerStay(other); break;
				case ECollisionEventState::EXIT: behaviour->OnTriggerExit(other); break;
			}
			return;
		}

		switch (state)
		{
			case ECollisionEventState::ENTER: behaviour->OnCollisionEnter(other); break;
			case ECollisionEventState::STAY: behaviour->OnCollisionStay(other); break;
			case ECollisionEventState::EXIT: behaviour->OnCollisionExit(other); break;
		}
	}

	void ServerCollisionManager::ResolveCollision(BoxCollider* lhs, BoxCollider* rhs)
	{
		if (!lhs || !rhs) return;
		const EColliderMobility lhsMobility = lhs->GetMobility();
		const EColliderMobility rhsMobility = rhs->GetMobility();

		if (lhsMobility == EColliderMobility::STATIC && rhsMobility == EColliderMobility::STATIC)
			return;

		if (lhsMobility == EColliderMobility::DYNAMIC && rhsMobility == EColliderMobility::DYNAMIC)
		{
			ResolveDynamicDynamic(lhs, rhs);
			return;
		}

		if (lhsMobility == EColliderMobility::STATIC && rhsMobility == EColliderMobility::DYNAMIC)
		{
			ResolveStaticDynamic(lhs, rhs);
			return;
		}

		if (lhsMobility == EColliderMobility::DYNAMIC && rhsMobility == EColliderMobility::STATIC)
		{
			ResolveStaticDynamic(rhs, lhs);
			return;
		}
	}

	void ServerCollisionManager::ResolveStaticDynamic(BoxCollider* staticCollider, BoxCollider* dynamicCollider)
	{
		if (!staticCollider || !dynamicCollider) return;

		CharacterBody2D* dynamicBody = GetCharacterBody(dynamicCollider);
		if (!dynamicBody) return;

		switch (staticCollider->GetCollisionType())
		{
			case ECollisionType::SOLID:
				ResolveSolidStaticDynamic(staticCollider, dynamicCollider, dynamicBody);
				break;

			case ECollisionType::PLATFORM:
				ResolvePlatformStaticDynamic(staticCollider, dynamicCollider, dynamicBody);
				break;
		}
	}

	void ServerCollisionManager::ResolveDynamicDynamic(BoxCollider* lhs, BoxCollider* rhs)
	{
		if (!lhs || !rhs) return;

		CharacterBody2D* lhsBody = GetCharacterBody(lhs);
		CharacterBody2D* rhsBody = GetCharacterBody(rhs);
		if (!lhsBody || !rhsBody) return;

		CollisionResolveInfo info;
		if (!GetCollisionResolveInfo(lhs, rhs, info)) return;

		const float lhsInvMass = 1.0f / lhsBody->GetMass();
		const float rhsInvMass = 1.0f / rhsBody->GetMass();
		const float totalInvMass = lhsInvMass + rhsInvMass;
		if (totalInvMass <= 0.0f) return;

		const float lhsRatio = lhsInvMass / totalInvMass;
		const float rhsRatio = rhsInvMass / totalInvMass;

		// Vertical
		if (info.lhsCenter.y < info.rhsCenter.y)
		{
			const float penetration = info.intersectionSize.y;
			const float lhsCorrection = penetration * lhsRatio;
			const float rhsCorrection = penetration * rhsRatio;

			if (info.lhsCenter.y < info.rhsCenter.y)
			{
				lhsBody->ApplyPosition(Shared::vec3(0.0f, -lhsCorrection, 0.0f));
				rhsBody->ApplyPosition(Shared::vec3(0.0f, rhsCorrection, 0.0f));
			}
			else
			{
				lhsBody->ApplyPosition(Shared::vec3(0.0f, lhsCorrection, 0.0f));
				rhsBody->ApplyPosition(Shared::vec3(0.0f, -rhsCorrection, 0.0f));
			}
		}

		// Horizontal
		else
		{
			const float penetration = info.intersectionSize.x;
			const float lhsCorrection = penetration * lhsRatio;
			const float rhsCorrection = penetration * rhsRatio;

			if (info.lhsCenter.x < info.rhsCenter.x)
			{
				lhsBody->ApplyPosition(Shared::vec3(-lhsCorrection, 0.0f, 0.0f));
				rhsBody->ApplyPosition(Shared::vec3(rhsCorrection, 0.0f, 0.0f));
			}
			else
			{
				lhsBody->ApplyPosition(Shared::vec3(lhsCorrection, 0.0f, 0.0f));
				rhsBody->ApplyPosition(Shared::vec3(-rhsCorrection, 0.0f, 0.0f));
			}
		}

		lhs->UpdateBounds();
		rhs->UpdateBounds();
	}

	void ServerCollisionManager::ResolveSolidStaticDynamic(BoxCollider* staticCollider, BoxCollider* dynamicCollider, CharacterBody2D* dynamicBody)
	{
		if (!staticCollider || !dynamicCollider || !dynamicBody) return;

		CollisionResolveInfo info;
		if (!GetCollisionResolveInfo(dynamicCollider, staticCollider, info)) return;

		// Vertical Collision
		if (info.intersectionSize.y < info.intersectionSize.x)
		{
			if (info.lhsCenter.y > info.rhsCenter.y) ResolveBottomStaticDynamic(staticCollider, dynamicCollider, dynamicBody);
			else ResolveTopStaticDynamic(staticCollider, dynamicCollider, dynamicBody);
			return;
		}

		// Horizontal Collision
		if (info.lhsCenter.x < info.rhsCenter.x) ResolveRightStaticDynamic(staticCollider, dynamicCollider, dynamicBody);
		else ResolveLeftStaticDynamic(staticCollider, dynamicCollider, dynamicBody);
	}

	void ServerCollisionManager::ResolvePlatformStaticDynamic(BoxCollider* staticCollider, BoxCollider* dynamicCollider, CharacterBody2D* dynamicBody)
	{
		if (!staticCollider || !dynamicCollider || !dynamicBody) return;

		// 위로 이동 중이면 플랫폼 통과
		if (dynamicBody->GetVerticalVelocity() > 0.0f)
			return;

		const AABB& dynamicBounds = dynamicCollider->GetBounds();
		const AABB& staticBounds = staticCollider->GetBounds();

		// 아직 플랫폼 아래쪽이라면 충돌하지 않음
		if (dynamicBounds.GetCenter().y < staticBounds.GetCenter().y)
			return;

		ResolveBottomStaticDynamic(staticCollider, dynamicCollider, dynamicBody);
	}

	void ServerCollisionManager::ResolveBottomStaticDynamic(BoxCollider* staticCollider, BoxCollider* dynamicCollider, CharacterBody2D* dynamicBody)
	{
		if (!staticCollider || !dynamicCollider || !dynamicBody) return;

		const float penetration = staticCollider->GetBounds().max.y - dynamicCollider->GetBounds().min.y;

		dynamicBody->ApplyPosition(Shared::vec3(0.0f, penetration, 0.0f));

		if (dynamicBody->GetVerticalVelocity() < 0.0f)
			dynamicBody->SetVerticalVelocity(0.0f);

		dynamicBody->SetGrounded(true);
		dynamicCollider->UpdateBounds();
	}

	void ServerCollisionManager::ResolveTopStaticDynamic(BoxCollider* staticCollider, BoxCollider* dynamicCollider, CharacterBody2D* dynamicBody)
	{
		if (!staticCollider || !dynamicCollider || !dynamicBody) return;

		const float penetration = dynamicCollider->GetBounds().max.y - staticCollider->GetBounds().min.y;

		dynamicBody->ApplyPosition(Shared::vec3(0.0f, -penetration, 0.0f));

		if (dynamicBody->GetVerticalVelocity() > 0.0f)
			dynamicBody->SetVerticalVelocity(0.0f);

		dynamicCollider->UpdateBounds();
	}

	void ServerCollisionManager::ResolveLeftStaticDynamic(BoxCollider* staticCollider, BoxCollider* dynamicCollider, CharacterBody2D* dynamicBody)
	{
		if (!staticCollider || !dynamicCollider || !dynamicBody) return;

		const float penetration = staticCollider->GetBounds().max.x - dynamicCollider->GetBounds().min.x;

		dynamicBody->ApplyPosition(Shared::vec3(penetration, 0.0f, 0.0f));

		if (dynamicBody->GetHorizontalVelocity() < 0.0f)
			dynamicBody->SetHorizontalVelocity(0.0f);

		dynamicCollider->UpdateBounds();
	}

	void ServerCollisionManager::ResolveRightStaticDynamic(BoxCollider* staticCollider, BoxCollider* dynamicCollider, CharacterBody2D* dynamicBody)
	{
		if (!staticCollider || !dynamicCollider || !dynamicBody) return;

		const float penetration = dynamicCollider->GetBounds().max.x - staticCollider->GetBounds().min.x;

		dynamicBody->ApplyPosition(Shared::vec3(-penetration, 0.0f, 0.0f));

		if (dynamicBody->GetHorizontalVelocity() > 0.0f)
			dynamicBody->SetHorizontalVelocity(0.0f);

		dynamicCollider->UpdateBounds();
	}
}
