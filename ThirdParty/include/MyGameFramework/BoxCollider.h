#pragma once
#include "Component.h"
#include "DebugGizmo.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(BoxCollider)

	/*=======================//
	//   Client BoxCollider  //
	//=======================*/
	class BoxCollider : public Component
	{
		MGSL_DISABLE_COPY(BoxCollider)
		MGSL_DISABLE_MOVE(BoxCollider)
		using Super = Component;
	
	public:
		virtual ~BoxCollider() override;
		static BoxColliderUPtr Create(GameObject* owner);
		void Update(float deltaTime) override;

	public:
		void SetServerPosition(const Shared::vec3& position);
		void SetTrigger(bool isTrigger);
		bool IsTrigger() const;

	private:
		explicit BoxCollider(GameObject* owner);

		Shared::Nullable<Shared::vec3> m_serverPosition;
		bool m_isTrigger = false;

		float m_ignoreThreshold = 0.1f;
		float m_snapThreshold = 1.0f;
		float m_correctionSpeed = 5.0f;

	public:
		void SetDebugSize(const Shared::vec2& size);
		void SetDebugOffset(const Shared::vec2& offset);
		DebugGizmo GetDebugGizmo() const;

	private:
		Shared::vec2 m_debugSize{ 1.0f, 1.0f };
		Shared::vec2 m_debugOffset{ 0.0f, 0.0f };
	};
}

