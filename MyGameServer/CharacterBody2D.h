#pragma once
#include "Component.h"

namespace MGSL::Server
{
	MGSL_CLASS_PTR(BoxCollider)
	MGSL_CLASS_PTR(CharacterBody2D)

	/*===========================//
	//   Server CharacterBody2D  //
	//===========================*/
	class CharacterBody2D : public Component
	{
		MGSL_DISABLE_COPY(CharacterBody2D)
		MGSL_DISABLE_MOVE(CharacterBody2D)
		using Super = Component;

	public:
		virtual ~CharacterBody2D() override;
		static CharacterBody2DUPtr Create(GameObject* owner);

	public:
		void Update(float deltaTime) override;
		void ApplyPosition(const Shared::vec3& pos);
		void Jump(float power);

	public:
		void SetHorizontalVelocity(float velocity);
		void SetVerticalVelocity(float velocity);
		float GetHorizontalVelocity() const;
		float GetVerticalVelocity() const;
		void SetGrounded(bool grounded);
		bool IsGrounded() const;

	public:
		void SetGravity(float gravity);
		float GetGravity() const;
		void SetGravityEnabled(bool enabled);
		bool IsGravityEnabled() const;
		void SetIgnorePlatform(bool ignore);
		bool IsIgnoringPlatform() const;
		void SetMass(float mass);
		float GetMass() const;

	private:
		explicit CharacterBody2D(GameObject* owner);

		float m_mass = 1.0f;
		float m_velocityX = 0.0f;
		float m_velocityY = 0.0f;
		float m_gravity = -9.8f;

		bool m_isGrounded = false;
		bool m_isGravityEnabled = true;
		bool m_ignorePlatform = false;
	};
}

