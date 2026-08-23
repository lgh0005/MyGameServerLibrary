#include "ServerPch.h"
#include "PlayerController.h"
#include "GameObject.h"
#include "CharacterBody2D.h"

namespace MGSL::Server
{
    PlayerController::PlayerController(GameObject* owner) : Super(owner) { }
    PlayerController::~PlayerController() = default;

    PlayerControllerUPtr PlayerController::Create(GameObject* owner)
    {
        return PlayerControllerUPtr(new PlayerController(owner));
    }

    void PlayerController::Update(float deltaTime)
    {
        GameObject* owner = GetOwner();
        if (!owner) return;

        CharacterBody2D* body = owner->GetComponent<CharacterBody2D>();
        if (!body) return;

        float directionX = 0.0f;
        switch (m_moveDirection)
        {
            case ::Protobuf::DIR_TYPE_LEFT:
                directionX = -1.0f;
                m_facing = Protobuf::FACING_TYPE_LEFT;
                break;

            case ::Protobuf::DIR_TYPE_RIGHT:
                directionX = 1.0f;
                m_facing = Protobuf::FACING_TYPE_RIGHT;
                break;

            default:
                break;
        }

        if (!body->IsGrounded())
        {
            if (body->GetVerticalVelocity() > 0.0f) m_state = Protobuf::OBJECT_STATE_TYPE_JUMP;
            else m_state = Protobuf::OBJECT_STATE_TYPE_FALL;
        }
        else
        {
            if (directionX != 0.0f)
            {
                if (m_isRunning)  m_state = Protobuf::OBJECT_STATE_TYPE_RUN;
                else m_state = Protobuf::OBJECT_STATE_TYPE_WALK;
            }
            else
            {
                m_state = Protobuf::OBJECT_STATE_TYPE_IDLE;
            }
        }

        if (directionX != 0.0f)
        {
            const float moveSpeed = m_isRunning ? m_runSpeed : m_moveSpeed;
            owner->GetTransform().Translate(Shared::vec3(directionX * moveSpeed * deltaTime, 0.0f, 0.0f));
        }
    }

    void PlayerController::SetMoveDirection(::Protobuf::DIR_TYPE dir)
    {
        m_moveDirection = dir;
    }

    void PlayerController::SetRunning(bool running)
    {
        m_isRunning = running;
    }

    void PlayerController::SetWeapon(Protobuf::WEAPON_TYPE weapon)
    {
        m_weapon = weapon;
    }

    ::Protobuf::DIR_TYPE PlayerController::GetMoveDirection() const
    {
        return m_moveDirection;
    }

    Protobuf::OBJECT_STATE_TYPE PlayerController::GetState() const
    {
        return m_state;
    }

    Protobuf::FACING_TYPE PlayerController::GetFacing() const
    {
        return m_facing;
    }

    Protobuf::WEAPON_TYPE PlayerController::GetWeapon() const
    {
        return m_weapon;
    }
}