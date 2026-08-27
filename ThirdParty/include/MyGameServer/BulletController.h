#pragma once
#include "ClientBehaviour.h"
#include "INetworkState.h"

namespace MGSL::Server
{
	MGSL_CLASS_PTR(BoxCollider)
	MGSL_CLASS_PTR(BulletController)

	class BulletController : public ClientBehaviour, public INetworkState<::Protobuf::BulletInfo>
	{
		MGSL_DISABLE_COPY(BulletController)
		MGSL_DISABLE_MOVE(BulletController)
		using Super = ClientBehaviour;

	public:
		virtual ~BulletController() override;
		static BulletControllerUPtr Create(GameObject* owner);

	public:
		virtual void Update(float deltaTime) override;
		virtual void OnTriggerEnter(BoxCollider* other) override;

	private:
		void SetVelocity();
		void DestroyBullet();

		float m_bulletSpeed = 7.5f;
		float m_elapsedTime = 0.0f;
		float m_lifeTime = 3.0f;

	/*=======================================//
	//   INetworkState interface overrides   //
	//=======================================*/
	public:
		virtual Shared::uint64 GetObjectID() const override { return m_info.objectid(); }
		virtual void SetObjectID(Shared::uint64 objectID) override { m_info.set_objectid(objectID); }
		virtual void SetInfo(const ::Protobuf::BulletInfo& info) override;
		virtual void SetInfo(::Protobuf::BulletInfo&& info) override;
		virtual ::Protobuf::BulletInfo& GetInfo() noexcept override { return m_info; }
		virtual const ::Protobuf::BulletInfo& GetInfo() const noexcept override { return m_info; }

	private:
		explicit BulletController(GameObject* owner);

		::Protobuf::BulletInfo m_info;
	};
}

