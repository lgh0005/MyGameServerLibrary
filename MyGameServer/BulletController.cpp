#include "ServerPch.h"
#include "BulletController.h"

namespace MGSL::Server
{
	BulletController::BulletController(GameObject* owner) : Super(owner) { }
	BulletController::~BulletController() = default;

	BulletControllerUPtr BulletController::Create(GameObject* owner)
	{
		return BulletControllerUPtr(new BulletController(owner));
	}

	Shared::uint64 BulletController::GetObjectID() const
	{
		return m_info.objectid();
	}

	void BulletController::SetObjectID(Shared::uint64 objectID)
	{
		m_info.set_objectid(objectID);
	}

	void BulletController::SetInfo(const ::Protobuf::BulletInfo& info)
	{
		m_info = info;
	}

	void BulletController::SetInfo(::Protobuf::BulletInfo&& info)
	{
		m_info = std::move(info);
	}

	::Protobuf::BulletInfo& BulletController::GetInfo() noexcept
	{
		return m_info;
	}

	const ::Protobuf::BulletInfo& BulletController::GetInfo() const noexcept
	{
		return m_info;
	}
}
