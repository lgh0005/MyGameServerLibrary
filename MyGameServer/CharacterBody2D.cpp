#include "ServerPch.h"
#include "CharacterBody2D.h"
#include "GameObject.h"

namespace MGSL::Server
{
	/*===========================//
	//   Server CharacterBody2D  //
	//===========================*/
	CharacterBody2D::CharacterBody2D(GameObject* owner) : Super(owner) { }
	CharacterBody2D::~CharacterBody2D() = default;

	CharacterBody2DUPtr CharacterBody2D::Create(GameObject* owner)
	{
		return CharacterBody2DUPtr(new CharacterBody2D(owner));
	}

	void CharacterBody2D::Update(float deltaTime)
	{
		// 이번 프레임 충돌 결과를 통해 다시 결정
		m_isGrounded = false;

		// 중력 적용
		m_velocityY += m_gravity * deltaTime;

		// 실제 이동
		GetTransform().Translate(Shared::vec3(m_velocityX * deltaTime, m_velocityY * deltaTime, 0.0f));
	}

	void CharacterBody2D::ApplyPosition(const Shared::vec3& pos)
	{
		GetTransform().Translate(Shared::vec3(pos.x, pos.y, 0.0f));
	}

	void CharacterBody2D::Jump(float power)
	{
		if (!m_isGrounded) return;
		m_velocityY = power;
		m_isGrounded = false;
	}

	/*============================//
	//   Velocity State Control   //
	//============================*/
	void CharacterBody2D::SetHorizontalVelocity(float velocity) { m_velocityX = velocity; }
	void CharacterBody2D::SetVerticalVelocity(float velocity) { m_velocityY = velocity; }
	float CharacterBody2D::GetHorizontalVelocity() const { return m_velocityX; }
	float CharacterBody2D::GetVerticalVelocity() const { return m_velocityY; }

	/*============================//
	//    Ground State Control    //
	//============================*/
	void CharacterBody2D::SetGrounded(bool grounded) { m_isGrounded = grounded; }
	bool CharacterBody2D::IsGrounded() const { return m_isGrounded; }

	/*============================//
	//       Physics States       //
	//============================*/
	void CharacterBody2D::SetGravity(float gravity) { m_gravity = gravity; }
	float CharacterBody2D::GetGravity() const { return m_gravity; }
	void CharacterBody2D::SetMass(float mass) { if (mass <= 0.0f) return; m_mass = mass; }
	float CharacterBody2D::GetMass() const { return m_mass; }
}