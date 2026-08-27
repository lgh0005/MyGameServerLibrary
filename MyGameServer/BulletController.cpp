#include "ServerPch.h"
#include "BulletController.h"
#include "GameRoom.h"
#include "PlayerController.h"
#include "BoxCollider.h"

namespace MGSL::Server
{
	BulletController::BulletController(GameObject* owner) : Super(owner) { }
	BulletController::~BulletController() = default;

	BulletControllerUPtr BulletController::Create(GameObject* owner)
	{
		return BulletControllerUPtr(new BulletController(owner));
	}

	void BulletController::Update(float deltaTime)
	{
		GameObject* owner = GetOwner();
		if (!owner) return;

		m_elapsedTime += deltaTime;
		if (m_elapsedTime >= m_lifeTime)
		{
			DestroyBullet();
			return;
		}

		const auto& velocity = m_info.velocity();
		const Shared::vec3 deltaPosition(velocity.x() * deltaTime, velocity.y() * deltaTime, 0.0f);

		owner->GetTransform().Translate(deltaPosition);
		const Shared::vec3& position = owner->GetTransform().GetPosition();
		m_info.mutable_position()->set_x(position.x);
		m_info.mutable_position()->set_y(position.y);
	}

	void BulletController::OnTriggerEnter(BoxCollider* other)
	{
		if (!other) return;

		switch (other->GetCollisionLayer())
		{
		case ECollisionLayer::WORLD:
		{
			DestroyBullet();
			break;
		}

		case ECollisionLayer::PLAYER:
		{
			GameObject* targetObject = other->GetOwner();
			if (!targetObject) return;

			PlayerController* targetController =
				targetObject->GetComponent<PlayerController>();

			if (!targetController) return;

			// 자신이 쏜 총알이면 무시
			if (targetController->GetObjectID() == m_info.ownerid())
				return;

			// 공격자 찾기
			GameObject* bulletObject = GetOwner();
			if (!bulletObject) return;

			Net::GameRoomPtr room = bulletObject->GetGameRoom();
			if (!room) return;

			PlayerController* attacker = room->FindPlayer(m_info.ownerid());
			if (!attacker) return;

			targetController->TakeDamage(1, attacker);

			DestroyBullet();
			break;
		}

		default:
			break;
		}
	}

	void BulletController::DestroyBullet()
	{
		Net::GameRoomPtr room = GetOwner()->GetGameRoom();
		if (!room) return;
		room->RemoveBullet(GetObjectID());
	}

	void BulletController::SetVelocity()
	{
		const float directionX = m_info.facing() == ::Protobuf::FACING_TYPE_LEFT ? -1.0f : 1.0f;
		m_info.mutable_velocity()->set_x(m_bulletSpeed * directionX);
		m_info.mutable_velocity()->set_y(0.0f);
	}

	/*=======================================//
	//   INetworkState interface overrides   //
	//=======================================*/
	void BulletController::SetInfo(const ::Protobuf::BulletInfo& info)
	{
		m_info = info;
		SetVelocity();
	}

	void BulletController::SetInfo(::Protobuf::BulletInfo&& info)
	{
		m_info = std::move(info);
		SetVelocity();
	}
}
