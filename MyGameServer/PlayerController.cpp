#include "ServerPch.h"
#include "PlayerController.h"
#include "GameObject.h"
#include "CharacterBody2D.h"
#include "BoxCollider.h"

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

        float directionX = 0.0f;
        switch (m_moveDirection)
        {
            case ::Protobuf::DIR_TYPE_LEFT:
                directionX = -1.0f;
                break;

            case ::Protobuf::DIR_TYPE_RIGHT:
                directionX = 1.0f;
                break;

            default:
                break;
        }

        if (directionX != 0.0f)
            owner->GetTransform().Translate(Shared::vec3(directionX * m_moveSpeed * deltaTime, 0.0f, 0.0f));
    }

    void PlayerController::OnCollisionEnter(BoxCollider* other)
    {
        if (!other) return;
        // TODO : gameplay collision event
    }

    void PlayerController::OnCollisionStay(BoxCollider* other)
    {
        if (!other) return;
        // TODO : gameplay collision event
    }

    void PlayerController::OnCollisionExit(BoxCollider* other)
    {
        // TODO
    }

    void PlayerController::SetMoveDirection(::Protobuf::DIR_TYPE dir)
    {
        m_moveDirection = dir;
    }

    ::Protobuf::DIR_TYPE PlayerController::GetMoveDirection() const
    {
        return m_moveDirection;
    }
}