#include "ServerPch.h"
#include "AABB.h"

namespace MGSL::Server
{
	bool AABB::Intersects(const AABB& other) const
	{
		return
			min.x <= other.max.x &&
			max.x >= other.min.x &&
			min.y <= other.max.y &&
			max.y >= other.min.y;
	}

	Shared::vec2 AABB::GetCenter() const
	{
		return (min + max) * 0.5f;
	}

	Shared::vec2 AABB::GetSize() const
	{
		return max - min;
	}

	bool AABB::IsValid() const
	{
		return min.x <= max.x && min.y <= max.y;
	}

	AABB AABB::operator&(const AABB& other) const
	{
		AABB result;
		result.min.x = glm::max(min.x, other.min.x);
		result.min.y = glm::max(min.y, other.min.y);
		result.max.x = glm::min(max.x, other.max.x);
		result.max.y = glm::min(max.y, other.max.y);
		return result;
	}

	bool AABB::operator<(const AABB& other) const
	{
		return min.x < other.min.x;
	}

	bool AABB::operator>(const AABB& other) const
	{
		return min.x > other.min.x;
	}

	bool AABB::operator<=(const AABB& other) const
	{
		return min.x <= other.min.x;
	}

	bool AABB::operator>=(const AABB& other) const
	{
		return min.x >= other.min.x;
	}
}
