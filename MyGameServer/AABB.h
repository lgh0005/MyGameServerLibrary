#pragma once

namespace MGSL::Server
{
	struct AABB
	{
		Shared::vec2 min{ 0.0f };
		Shared::vec2 max{ 0.0f };
		bool Intersects(const AABB& other) const;
		Shared::vec2 GetCenter() const;
		Shared::vec2 GetSize() const;
		bool IsValid() const;

		AABB operator&(const AABB& other) const;
		bool operator<(const AABB& other) const;
		bool operator>(const AABB& other) const;
		bool operator<=(const AABB& other) const;
		bool operator>=(const AABB& other) const;
	};
}

