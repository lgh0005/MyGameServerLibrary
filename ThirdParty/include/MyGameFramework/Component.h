#pragma once
#include "Transform.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(GameObject)

	class Component
	{
		MGSL_DISABLE_COPY(Component)
		MGSL_DISABLE_MOVE(Component)

	public:
		virtual ~Component();

	public:
		virtual void Awake();
		virtual void Start();
		virtual void Update(float deltaTime);
		virtual void LateUpdate(float deltaTime);
		virtual void Submit();

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

