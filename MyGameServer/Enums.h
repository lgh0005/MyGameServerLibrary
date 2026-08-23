#pragma once

namespace MGSL::Server
{
	enum class ECollisionEventState : Shared::uint8
	{
		ENTER,
		STAY,
		EXIT
	};

	enum class EColliderMobility : Shared::uint8
	{
		STATIC,
		DYNAMIC
	};

	enum class ECollisionType : Shared::uint8
	{
		SOLID,
		PLATFORM
	};

	enum class ECollsionLayer : Shared::uint8
	{
		PLAYER,
		ENEMY,
		WORLD,
		LADDER,
		BULLET,
		HITBOX
	};
}