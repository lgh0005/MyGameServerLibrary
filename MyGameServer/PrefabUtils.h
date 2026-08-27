#pragma once

namespace MGSL::Server
{
	class PrefabUtils
	{
		MGSL_DISABLE_COPY(PrefabUtils)
		MGSL_DISABLE_MOVE(PrefabUtils)

	public:
		static GameObject* CreateBullet(VirtualScene* scene, const ::Protobuf::BulletInfo& info);
	};
}

