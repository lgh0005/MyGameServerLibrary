#pragma once

namespace MGSL::Sandbox2D
{
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
		FIGHTER,
		PISTOL,
		SWORD,
		COUNT
	};
}