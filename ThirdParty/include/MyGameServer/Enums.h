#pragma once

namespace MGSL::Server
{
	enum class ECollisionEventState
	{
		ENTER,
		STAY,
		EXIT
	};

	enum class EColliderMobility
	{
		STATIC,
		DYNAMIC
	};

	enum class ECollisionType
	{
		SOLID,
		PLATFORM
	};
}