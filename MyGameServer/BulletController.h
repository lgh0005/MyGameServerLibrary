#pragma once
#include "ClientBehaviour.h"
#include "INetworkState.h"

namespace MGSL::Server
{
	MGSL_CLASS_PTR(BulletController)

	class BulletController : public ClientBehaviour, public INetworkState<::Protobuf::BulletInfo>
	{
		MGSL_DISABLE_COPY(BulletController)
		MGSL_DISABLE_MOVE(BulletController)
		using Super = ClientBehaviour;

	public:
		virtual ~BulletController() override;
		static BulletControllerUPtr Create(GameObject* owner);

	/*=======================================//
	//   INetworkState interface overrides   //
	//=======================================*/
	public:
		virtual Shared::uint64 GetObjectID() const override;
		virtual void SetObjectID(Shared::uint64 objectID) override;
		virtual void SetInfo(const ::Protobuf::BulletInfo& info) override;
		virtual void SetInfo(::Protobuf::BulletInfo&& info) override;
		virtual ::Protobuf::BulletInfo& GetInfo() noexcept override;
		virtual const ::Protobuf::BulletInfo& GetInfo() const noexcept override;

	private:
		explicit BulletController(GameObject* owner);

		::Protobuf::BulletInfo m_info;
	};
}

