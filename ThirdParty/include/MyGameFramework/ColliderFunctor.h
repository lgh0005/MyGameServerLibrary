#pragma once

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(BoxCollider)

	struct CollisionPair
	{
		BoxCollider* first = nullptr;
		BoxCollider* second = nullptr;
		CollisionPair();
		CollisionPair(BoxCollider* lhs, BoxCollider* rhs);
		bool operator==(const CollisionPair& other) const;
	};

	struct BoxColliderLess
	{
		bool operator()(const BoxCollider* lhs, const BoxCollider* rhs) const;
	};

	struct BoxColliderGreater
	{
		bool operator()(const BoxCollider* lhs, const BoxCollider* rhs) const;
	};
}
