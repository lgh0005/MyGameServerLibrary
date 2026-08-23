#pragma once
#include "Transform.h"

namespace MGSL::Server
{
	MGSL_CLASS_PTR(GameObject)

	class Component
	{
		MGSL_DISABLE_COPY(Component)
		MGSL_DISABLE_MOVE(Component)

	public:
		virtual ~Component();

	public:
		virtual void Update(float deltaTime);
		virtual void LateUpdate(float deltaTime);

	public:
		GameObject* GetOwner();
		const GameObject* GetOwner() const;

	protected:
		Component(GameObject* owner);

		Transform& GetTransform();
		const Transform& GetTransform() const;

	private:
		GameObject* m_owner = nullptr;
	};
}

