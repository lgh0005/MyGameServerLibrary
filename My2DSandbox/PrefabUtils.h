#pragma once

namespace MGSL::Sandbox2D
{
	/*================================//
	//   prefab util static methods   //
	//================================*/
	class PrefabUtils
	{
		MGSL_DISABLE_COPY(PrefabUtils)
		MGSL_DISABLE_MOVE(PrefabUtils)

	public:
		static Framework::GameObject* CreateBullet(Framework::Scene* scene, const ::Protobuf::BulletInfo& info);
		static Framework::GameObject* CreateAttackEffect(Framework::Scene* scene, const Shared::vec3& position, ::Protobuf::FACING_TYPE facing);
	};
}

