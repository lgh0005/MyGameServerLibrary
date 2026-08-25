#include "ServerPch.h"
#include "PlayerController.h"
#include "GameObject.h"
#include "CharacterBody2D.h"
#include "BoxCollider.h"

namespace MGSL::Server
{
    PlayerController::PlayerController(GameObject* owner) : Super(owner)
    {
        m_info.set_state(::Protobuf::OBJECT_STATE_TYPE_IDLE);
        m_info.set_facing(::Protobuf::FACING_TYPE_RIGHT);
        m_info.set_weapon(::Protobuf::WEAPON_TYPE_NONE);
    }
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

        /*========================//
        //         Attack         //
        //========================*/
        if (m_isAttacking)
        {
            m_attackElapsedTime += deltaTime;
            if (m_attackElapsedTime < m_attackDuration) return;
            if (m_attackQueued && m_comboIndex < 3)
            {
                ++m_comboIndex;
                switch (m_comboIndex)
                {
                    case 2: m_info.set_state(::Protobuf::OBJECT_STATE_TYPE_ATTACK_2); break;
                    case 3: m_info.set_state(::Protobuf::OBJECT_STATE_TYPE_ATTACK_3); break;
                    default: break;
                }

                m_attackQueued = false;
                m_attackElapsedTime = 0.0f;
                m_hitTargets.clear();
                return;
            }

            m_isAttacking = false;
            m_attackQueued = false;
            m_comboIndex = 0;
            m_attackElapsedTime = 0.0f;
            m_hitTargets.clear();
        }

        /*========================//
        //        Movement        //
        //========================*/
        float directionX = 0.0f;
        switch (m_moveDirection)
        {
            case ::Protobuf::DIR_TYPE_LEFT:
                directionX = -1.0f;
                m_info.set_facing(Protobuf::FACING_TYPE_LEFT);
                break;

            case ::Protobuf::DIR_TYPE_RIGHT:
                directionX = 1.0f;
                m_info.set_facing(Protobuf::FACING_TYPE_RIGHT);
                break;

            default:
                break;
        }

        /*========================//
        //          State         //
        //========================*/
        if (!body->IsGrounded())
        {
            if (body->GetVerticalVelocity() > 0.0f) m_info.set_state(::Protobuf::OBJECT_STATE_TYPE_JUMP);
            else m_info.set_state(::Protobuf::OBJECT_STATE_TYPE_FALL);
        }
        else
        {
            if (directionX != 0.0f)
            {
                if (m_isRunning)  m_info.set_state(::Protobuf::OBJECT_STATE_TYPE_RUN);
                else m_info.set_state(::Protobuf::OBJECT_STATE_TYPE_WALK);
            }
            else
            {
                m_info.set_state(::Protobuf::OBJECT_STATE_TYPE_IDLE);
            }
        }

        /*========================//
        //      Translation       //
        //========================*/
        if (directionX != 0.0f)
        {
            const float moveSpeed = m_isRunning ? m_runSpeed : m_moveSpeed;
            owner->GetTransform().Translate(Shared::vec3(directionX * moveSpeed * deltaTime, 0.0f, 0.0f));
        }
    }

    void PlayerController::OnTriggerEnter(BoxCollider* other)
    {
        HandleHitboxTrigger(other);
    }

    void PlayerController::OnTriggerStay(BoxCollider* other)
    {
        HandleHitboxTrigger(other);
    }

    void PlayerController::HandleHitboxTrigger(BoxCollider* other)
    {
        if (!other) return;
        if (other->GetCollisionLayer() != ECollisionLayer::HITBOX) return;

        GameObject* hitboxObject = other->GetOwner();
        if (!hitboxObject) return;

        GameObject* attacker = hitboxObject->GetParent();
        if (!attacker) return;

        PlayerController* attackerController = attacker->GetComponent<PlayerController>();
        if (!attackerController) return;

        // 실제 공격 중이 아니면 충돌은 있었어도 Hit로 인정하지 않음
        if (!attackerController->IsAttacking()) return;

        const Shared::uint64 targetID = GetObjectID();
        if (!attackerController->RegisterHitTarget(targetID))
            return;

        // DEBUG
        MGSL_LOG_INFO
        (
            "HITBOX collision detected. Attacker = {}, Target = {}",
            attackerController->GetObjectID(),
            targetID
        );
    }

    bool PlayerController::RegisterHitTarget(Shared::uint64 objectID)
    {
        auto [it, inserted] = m_hitTargets.insert(objectID);
        return inserted;
    }

    void PlayerController::Attack()
    {
        GameObject* owner = GetOwner();
        if (!owner) return;

        CharacterBody2D* body = owner->GetComponent<CharacterBody2D>();
        if (!body) return;

        /*========================//
        //      Combo Queue       //
        //========================*/
        if (m_isAttacking)
        {
            if (m_info.weapon() == ::Protobuf::WEAPON_TYPE_NONE ||
                m_info.weapon() == ::Protobuf::WEAPON_TYPE_SWORD)
            {
                const bool isComboWindow =
                    m_attackElapsedTime >= m_comboWindowStart &&
                    m_attackElapsedTime <= m_comboWindowEnd;

                if (m_comboIndex < 3 && isComboWindow)
                    m_attackQueued = true;
            }

            return;
        }

        /*========================//
        //      Attack Start      //
        //========================*/
        m_isAttacking = true;
        m_attackQueued = false;
        m_attackElapsedTime = 0.0f;

        if (!body->IsGrounded())
        {
            m_comboIndex = 0;
            m_info.set_state(::Protobuf::OBJECT_STATE_TYPE_AIR_ATTACK);
            return;
        }

        switch (m_info.weapon())
        {
            case Protobuf::WEAPON_TYPE_NONE:
            case Protobuf::WEAPON_TYPE_SWORD:
                m_comboIndex = 1;
                m_info.set_state(::Protobuf::OBJECT_STATE_TYPE_ATTACK_1);
                break;

            case Protobuf::WEAPON_TYPE_PISTOL:
                m_comboIndex = 0;
                m_info.set_state(::Protobuf::OBJECT_STATE_TYPE_SHOT);
                break;

            default:
                m_isAttacking = false;
                break;
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
        m_info.set_weapon(weapon);
    }

    ::Protobuf::DIR_TYPE PlayerController::GetMoveDirection() const
    {
        return m_moveDirection;
    }

    Protobuf::OBJECT_STATE_TYPE PlayerController::GetState() const
    {
        return m_info.state();
    }

    Protobuf::FACING_TYPE PlayerController::GetFacing() const
    {
        return m_info.facing();
    }

    Protobuf::WEAPON_TYPE PlayerController::GetWeapon() const
    {
        return m_info.weapon();
    }

    bool PlayerController::IsAttacking() const
    {
        return m_isAttacking;
    }

    Shared::uint64 PlayerController::GetObjectID() const
    {
        return m_info.objectid();
    }

    void PlayerController::SetObjectID(Shared::uint64 objectID)
    {
        m_info.set_objectid(objectID);
    }

    void PlayerController::SetInfo(const ::Protobuf::PlayerInfo& info)
    {
        m_info = info;
    }

    void PlayerController::SetInfo(::Protobuf::PlayerInfo&& info)
    {
        m_info = std::move(info);
    }

    ::Protobuf::PlayerInfo& PlayerController::GetInfo() noexcept
    {
        return m_info;
    }

    const ::Protobuf::PlayerInfo& PlayerController::GetInfo() const noexcept
    {
        return m_info;
    }
}
