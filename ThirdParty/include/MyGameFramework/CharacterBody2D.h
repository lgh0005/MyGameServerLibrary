#pragma once
#include "Component.h"

namespace MGSL::Framework
{
	MGSL_CLASS_PTR(BoxCollider)
	MGSL_CLASS_PTR(CharacterBody2D)

	/*===========================//
	//   Client CharacterBody2D  //
	//===========================*/
	class CharacterBody2D : public Component
	{
		MGSL_DISABLE_COPY(CharacterBody2D)
		MGSL_DISABLE_MOVE(CharacterBody2D)
		using Super = Component;

	public:
		virtual ~CharacterBody2D() override;
		static CharacterBody2DUPtr Create(GameObject* owner);
		void Update(float deltaTime) override;

	public:
		void SetHorizontalVelocity(float velocity);
		void SetVerticalVelocity(float velocity);
		float GetHorizontalVelocity() const;
		float GetVerticalVelocity() const;
		void SetServerVelocity(float velocityX, float velocityY);
		void SetServerGrounded(bool grounded);

	public:
		void Jump(float power);
		void SetGrounded(bool grounded);
		bool IsGrounded() const;
		void SetGravity(float gravity);
		float GetGravity() const;

	private:
		explicit CharacterBody2D(GameObject* owner);

		float m_velocityX = 0.0f;
		float m_velocityY = 0.0f;
		Shared::Nullable<float> m_serverVelocityX;
		Shared::Nullable<float> m_serverVelocityY;

		float m_gravity = -9.8f;
		bool m_isGrounded = false;
		bool m_serverGrounded = false;
	};
}

