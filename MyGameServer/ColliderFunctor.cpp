#include "ServerPch.h"
#include "ColliderFunctor.h"
#include "BoxCollider.h"
#include <functional>

namespace MGSL::Server
{
	CollisionPair::CollisionPair() = default;
	CollisionPair::CollisionPair(BoxCollider* lhs, BoxCollider* rhs)
	{
		if (std::less<BoxCollider*>{}(lhs, rhs))
		{
			first = lhs;
			second = rhs;
		}
		else
		{
			first = rhs;
			second = lhs;
		}
	}

	bool CollisionPair::operator==(const CollisionPair& other) const
	{
		return
			first == other.first &&
			second == other.second;
	}

	bool BoxColliderLess::operator()(const BoxCollider* lhs, const BoxCollider* rhs) const
	{
		return lhs->GetBounds() < rhs->GetBounds();
	}

	bool BoxColliderGreater::operator()(const BoxCollider* lhs, const BoxCollider* rhs) const
	{
		return lhs->GetBounds() > rhs->GetBounds();
	}
}
