#include "GameFrameworkPch.h"
#include "CharacterBody2D.h"
#include "GameObject.h"

namespace MGSL::Framework
{
	/*===========================//
	//   Client CharacterBody2D  //
	//===========================*/
	CharacterBody2D::CharacterBody2D(GameObject* owner) : Super(owner) { }
	CharacterBody2D::~CharacterBody2D() = default;

	CharacterBody2DUPtr CharacterBody2D::Create(GameObject* owner)
	{
		return CharacterBody2DUPtr(new CharacterBody2D(owner));
	}

	void CharacterBody2D::Update(float deltaTime)
	{ 
		/* 서버의 물리 상태를 기준으로 로컬 예측 상태 보정 */
		
		// Horizontal
		if (m_serverVelocityX.has_value())
		{
			const float serverVelocityX = m_serverVelocityX.value();
			const float differenceX = serverVelocityX - m_velocityX;
			if (std::abs(differenceX) > 1.0f) m_velocityX = serverVelocityX;
		}

		// Vertical
		if (m_serverVelocityY.has_value())
		{
			const float serverVelocityY = m_serverVelocityY.value();
			const float differenceY = serverVelocityY - m_velocityY;
			if (std::abs(differenceY) > 1.0f) m_velocityY = serverVelocityY;
		}

		m_isGrounded = m_serverGrounded;

		// 로컬 물리 Prediction
		if (!m_isGrounded) m_velocityY += m_gravity * deltaTime;
		else if (m_velocityY < 0.0f) m_velocityY = 0.0f;

		GetTransform().Translate(Shared::vec3(m_velocityX * deltaTime, m_velocityY * deltaTime, 0.0f));
	}

	void CharacterBody2D::SetHorizontalVelocity(float velocity)
	{
		m_velocityX = velocity;
	}

	void CharacterBody2D::SetVerticalVelocity(float velocity)
	{
		m_velocityY = velocity;
	}

	float CharacterBody2D::GetHorizontalVelocity() const
	{
		return m_velocityX;
	}

	float CharacterBody2D::GetVerticalVelocity() const
	{
		return m_velocityY;
	}

	void CharacterBody2D::SetServerVelocity(float velocityX, float velocityY)
	{
		m_serverVelocityX = velocityX;
		m_serverVelocityY = velocityY;
	}

	void CharacterBody2D::Jump(float power)
	{
		if (!m_isGrounded) return;
		m_velocityY = power;
		m_isGrounded = false;
	}

	void CharacterBody2D::SetGrounded(bool grounded)
	{
		m_isGrounded = grounded;
	}

	void CharacterBody2D::SetServerGrounded(bool grounded)
	{
		m_serverGrounded = grounded;
	}

	bool CharacterBody2D::IsGrounded() const
	{
		return m_isGrounded;
	}

	void CharacterBody2D::SetGravity(float gravity)
	{
		m_gravity = gravity;
	}

	float CharacterBody2D::GetGravity() const
	{
		return m_gravity;
	}
}

