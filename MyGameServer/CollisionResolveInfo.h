#pragma once
#include "AABB.h"

namespace MGSL::Server
{
	struct CollisionResolveInfo
	{
		AABB lhsBounds;
		AABB rhsBounds;
		AABB intersection;

		Shared::vec2 intersectionSize;
		Shared::vec2 lhsCenter;
		Shared::vec2 rhsCenter;
	};
}

