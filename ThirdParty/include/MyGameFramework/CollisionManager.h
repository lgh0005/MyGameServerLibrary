#pragma once

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(BoxCollider)
	MGSL_CLASS_PTR(MonoBehaviour)

	/*============================//
	//   Client CollisionManager  //
	//============================*/
	class CollisionManager
	{
		MGSL_DECLARE_SINGLE(CollisionManager)

	private:
		CollisionManager();
		~CollisionManager();

	public:
		void Init();
		void Update(float deltaTime);
		void Clear();

	public:
		void Register(BoxCollider* collider);
		void Unregister(BoxCollider* collider);

	public:
		const Shared::List<BoxCollider*> GetColliders() const;

	private:
		Shared::List<BoxCollider*> m_colliders;
	};
}

