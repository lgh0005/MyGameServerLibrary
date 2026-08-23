#pragma once
#include "Component.h"
#include "AABB.h"

namespace MGSL::Server
{
	MGSL_CLASS_PTR(BoxCollider)

	/*=======================//
	//   Server BoxCollider  //
	//=======================*/
	class BoxCollider : public Component
	{
		MGSL_DISABLE_COPY(BoxCollider)
		MGSL_DISABLE_MOVE(BoxCollider)
		using Super = Component;
	
	public:
		virtual ~BoxCollider() override;
		static BoxColliderUPtr Create(GameObject* owner);

	public:
		void SetSize(const Shared::vec2& size);
		void SetOffset(const Shared::vec2& offset);
		void SetMobility(EColliderMobility mobility);
		void SetCollisionType(ECollisionType type);
		void SetCollisionLayer(ECollisionLayer layer);
		void SetTrigger(bool isTrigger);

	public:
		const Shared::vec2& GetSize() const;
		const Shared::vec2& GetOffset() const;
		EColliderMobility GetMobility() const;
		ECollisionType GetCollisionType() const;
		ECollisionLayer GetCollisionLayer() const;
		bool IsTrigger() const;

	public:
		void UpdateBounds();
		const AABB& GetBounds() const;

	private:
		explicit BoxCollider(GameObject* owner);

		bool m_isTrigger = false;
		EColliderMobility m_mobility = EColliderMobility::DYNAMIC;
		ECollisionType m_collisoinType = ECollisionType::SOLID;
		ECollisionLayer m_collisionLayer = ECollisionLayer::WORLD;

		AABB m_bounds;
		Shared::vec2 m_size{ 1.0f, 1.0f };
		Shared::vec2 m_offset{ 0.0f, 0.0f };
	};
}

