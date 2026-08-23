#pragma once

namespace MGSL::Sandbox2D
{
	enum class EFacingDirection
	{
		LEFT,
		RIGHT
	};

	enum class EPlayerWeaponState
	{
		FIGHTER,
		PISTOL,
		SWORD
	};

	enum class EPlayerActionState
	{
		NONE,
		IDLE,
		WALK,
		RUN,
		JUMP,
		CLIMB,
		DASH,
		SLIDE,
		HIT,
		DEATH,
		AIR_ATTACK,
		COMBO1,
		COMBO2,
		COMBO3,
		SHOT
	};
}