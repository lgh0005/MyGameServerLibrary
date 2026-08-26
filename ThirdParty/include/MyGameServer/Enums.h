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

	enum class ECollisionLayer : Shared::uint8
	{
		PLAYER,
		ENEMY,
		WORLD,
		LADDER,
		BULLET,
		HITBOX,
		COUNT
	};

	enum class EObjectState : Shared::uint8
	{
		IDLE,
		WALK,
		RUN,
		JUMP,
		FALL,
		SLIDE,
		DASH,
		CLIMB,
		AIR_ATTACK,
		ATTACK_1,
		ATTACK_2,
		ATTACK_3,
		SHOT,
		HIT,
		DEATH,
		COUNT
	};

	enum class EFacingDirection : Shared::uint8
	{
		LEFT,
		RIGHT
	};

	enum class EWeaponType : Shared::uint8
	{
		NONE,
		PISTOL,
		SWORD,
		COUNT
	};

	enum class ELadderState : Shared::uint8
	{
		NONE,
		CONTACT,
		CLIMBING
	};
}