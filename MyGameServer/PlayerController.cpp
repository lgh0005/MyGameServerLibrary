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

        // TODO : Hard-coded value
        m_info.set_life(5);
        m_info.set_kill_cnt(0);
        m_info.set_invincible(true);
    }

    PlayerController::~PlayerController() = default;

    PlayerControllerUPtr PlayerController::Create(GameObject* owner)
    {
        return PlayerControllerUPtr(new PlayerController(owner));
    }

    void PlayerController::Update(float deltaTime)
    {
        GameObject* owner = GetOwner(); if (!owner) return;
        CharacterBody2D* body = owner->GetComponent<CharacterBody2D>(); if (!body) return;

        // Death
        if (UpdateDeath(deltaTime)) return;

        const float directionX = GetHorizontalDirectionValue();
        const float directionY = GetVerticalDirectionValue();

        // Hit
        UpdateHit(deltaTime);

        // Attack
        if (UpdateAttack(deltaTime)) return;
        
        // Land
        UpdateLand(body, deltaTime);
        if (IsLanding()) return;

        // Climb
        UpdateClimb(body, directionY);

        // Move
        UpdateMove(owner, directionX, deltaTime);

        // Player State
        UpdatePlayerState(body, directionX);
    }

    /*==============================================//
    //      Default player syncing behaviours       //
    //==============================================*/

#pragma region DEATH
    void PlayerController::Death()
    {
        if (m_isDead) return;

        m_isDead = true;

        // Network State
        m_info.set_life(0);
        m_info.set_invincible(false);
        m_info.set_state(::Protobuf::OBJECT_STATE_TYPE_DEATH);

        // Hit State
        m_isHit = false;
        m_hitElapsedTime = 0.0f;
        m_invincibleElapsedTime = 0.0f;

        // Attack State
        m_isAttacking = false;
        m_attackQueued = false;
        m_comboIndex = 0;
        m_attackElapsedTime = 0.0f;
        m_hitTargets.clear();

        // Climb State
        m_ladderState = ELadderState::NONE;

        // Move State
        m_horizontalDirection = ::Protobuf::DIR_TYPE_NONE;
        m_verticalDirection = ::Protobuf::DIR_TYPE_NONE;
        m_isRunning = false;
    }

    bool PlayerController::UpdateDeath(float /*deltaTime*/)
    {
        return IsDead();

        // TODO : 연결을 끊을 시간을 마련해준다.
        //if (!m_isDead)
        //    return false;

        //m_deathElapsedTime += deltaTime;

        //if (m_deathElapsedTime >= m_deathDuration)
        //{
        //    // Disconnect
        //}

        //return true;
    }
#pragma endregion

#pragma region HIT
    void PlayerController::Hit()
    {
        // 무적 상태에서는 피해 무시
        if (IsInvincible()) return;

        // 피해 상태 처리
        m_isHit = true;
        m_hitElapsedTime = 0.0f;
        m_invincibleElapsedTime = 0.0f;
        m_info.set_state(::Protobuf::OBJECT_STATE_TYPE_HIT);
        m_info.set_invincible(true);
    }

    void PlayerController::TakeDamage(Shared::uint32 damage, PlayerController* attacker)
    {
        if (damage <= 0) return;

        if (IsDead())
            return;

        // 무적 중에는 Damage 무시
        if (IsInvincible())
            return;

        const Shared::uint32 currentLife = m_info.life();
        if (currentLife <= 0) return;

        // Life 감소
        const Shared::uint32 nextLife = damage >= currentLife ? 0 : currentLife - damage;
        m_info.set_life(nextLife);

        // 사망
        if (nextLife == 0)
        {
            attacker->AddKill();
            Death();
            return;
        }

        // 살아있으면 Hit
        Hit();
    }

    void PlayerController::UpdateHit(float deltaTime)
    {
        // Hit 상태
        if (m_isHit)
        {
            m_hitElapsedTime += deltaTime;
            if (m_hitElapsedTime >= m_hitDuration)
            {
                m_isHit = false;
                m_hitElapsedTime = 0.0f;
            }
        }

        // Invincible 상태
        if (IsInvincible())
        {
            m_invincibleElapsedTime += deltaTime;
            if (m_invincibleElapsedTime >= m_invincibleDuration)
            {
                m_invincibleElapsedTime = 0.0f;
                m_info.set_invincible(false);
            }
        }
    }
#pragma endregion

#pragma region LAND
    void PlayerController::UpdateLand(CharacterBody2D* body, float deltaTime)
    {
        if (!body) return;

        const bool isGrounded = body->IsGrounded();

        // Landing State
        if (!m_wasGrounded && isGrounded)
        {
            m_isLanding = true;
            m_landElapsedTime = 0.0f;
            m_info.set_state(::Protobuf::OBJECT_STATE_TYPE_LAND);
        }

        if (m_isLanding)
        {
            m_landElapsedTime += deltaTime;
            if (m_landElapsedTime >= m_landDuration)
            {
                m_isLanding = false;
                m_landElapsedTime = 0.0f;
            }
        }

        m_wasGrounded = isGrounded;
    }


#pragma endregion

#pragma region MOVE
    void PlayerController::UpdateMove(GameObject* owner, float directionX, float deltaTime)
    {
        if (!owner || directionX == 0.0f) return;
        const float moveSpeed = m_isRunning ? m_runSpeed : m_moveSpeed;
        owner->GetTransform().Translate(Shared::vec3(directionX * moveSpeed * deltaTime, 0.0f, 0.0f));
    }

    float PlayerController::GetHorizontalDirectionValue()
    {
        switch (m_horizontalDirection)
        {
            case ::Protobuf::DIR_TYPE_LEFT:
                m_info.set_facing(::Protobuf::FACING_TYPE_LEFT);
                return -1.0f;

            case ::Protobuf::DIR_TYPE_RIGHT:
                m_info.set_facing(::Protobuf::FACING_TYPE_RIGHT);
                return 1.0f;

            default:
                return 0.0f;
        }
    }

    
    
    float PlayerController::GetVerticalDirectionValue()
    {
        switch (m_verticalDirection)
        {
            case ::Protobuf::DIR_TYPE_UP: return 1.0f;
            case ::Protobuf::DIR_TYPE_DOWN: return -1.0f;
            default: return 0.0f;
        }
    }
#pragma endregion

#pragma region CLIMB
    void PlayerController::UpdateClimb(CharacterBody2D* body, float directionY)
    {
        if (!body) return;

        switch (m_ladderState)
        {
            case ELadderState::NONE:
            {
                body->SetGravityEnabled(true);
                body->SetIgnorePlatform(false);
                break;
            }

            case ELadderState::CONTACT:
            {
                if (directionY == 0.0f) break;
                m_ladderState = ELadderState::CLIMBING;
                body->SetGravityEnabled(false);
                body->SetIgnorePlatform(true);
                body->SetVerticalVelocity(directionY * m_climbSpeed);
                break;
            }

            case ELadderState::CLIMBING:
            {
                body->SetGravityEnabled(false);
                body->SetIgnorePlatform(true);
                body->SetVerticalVelocity(directionY * m_climbSpeed);
                break;
            }
        }
    }
#pragma endregion

#pragma region ATTACK
    void PlayerController::Attack()
    {
        GameObject* owner = GetOwner(); if (!owner) return;
        CharacterBody2D* body = owner->GetComponent<CharacterBody2D>(); if (!body) return;

        // Combo Queue
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

        // Attack Start
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
            case ::Protobuf::WEAPON_TYPE_NONE:
            case ::Protobuf::WEAPON_TYPE_SWORD:
                m_comboIndex = 1;
                m_info.set_state(::Protobuf::OBJECT_STATE_TYPE_ATTACK_1);
                break;

            case ::Protobuf::WEAPON_TYPE_PISTOL:
                m_comboIndex = 0;
                m_info.set_state(::Protobuf::OBJECT_STATE_TYPE_SHOT);
                break;

            default:
                m_isAttacking = false;
                break;
        }
    }

    bool PlayerController::UpdateAttack(float deltaTime)
    {
        if (!m_isAttacking)
            return false;

        m_attackElapsedTime += deltaTime;

        // 아직 공격 동작 진행 중
        if (m_attackElapsedTime < m_attackDuration)
            return true;

        // 다음 콤보 진행
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
            return true;
        }

        // 공격 종료
        m_isAttacking = false;
        m_attackQueued = false;
        m_comboIndex = 0;
        m_attackElapsedTime = 0.0f;
        m_hitTargets.clear();
        return false;
    }

    bool PlayerController::RegisterHitTarget(Shared::uint64 objectID)
    {
        auto [it, inserted] = m_hitTargets.insert(objectID);
        return inserted;
    }
#pragma endregion

#pragma region COLLISION
    void PlayerController::OnTriggerEnter(BoxCollider* other) { HandleTrigger(other); }

    void PlayerController::OnTriggerStay(BoxCollider* other) { HandleTrigger(other); }

    void PlayerController::OnTriggerExit(BoxCollider* other)
    {
        if (!other) return;
        switch (other->GetCollisionLayer())
        {
            case ECollisionLayer::LADDER: m_ladderState = ELadderState::NONE; break;

            // TODO CASE ECollisionLayer::BULLET: break;

            default: break;
        }
    }

    void PlayerController::HandleTrigger(BoxCollider* other)
    {
        if (!other) return;

        switch (other->GetCollisionLayer())
        {
        case ECollisionLayer::HITBOX:
        {
            GameObject* hitboxObject = other->GetOwner(); if (!hitboxObject) return;
            GameObject* attacker = hitboxObject->GetParent(); if (!attacker) return;
            PlayerController* attackerController = attacker->GetComponent<PlayerController>(); if (!attackerController) return;

            // 실제 공격 중이 아니면 충돌은 있었어도 Hit로 인정하지 않음
            if (!attackerController->IsAttacking()) return;

            const Shared::uint64 targetID = GetObjectID();
            if (!attackerController->RegisterHitTarget(targetID))
                return;

            // Damage
            TakeDamage(1, attackerController);

            MGSL_LOG_INFO
            (
                "HITBOX collision detected. Attacker = {}, Target = {}",
                attackerController->GetObjectID(),
                targetID
            );

            break;
        }

        case ECollisionLayer::LADDER:
        {
            if (m_ladderState == ELadderState::NONE) m_ladderState = ELadderState::CONTACT;
            break;
        }

        default:
            break;
        }
    }
#pragma endregion

#pragma region PLYAER_STATE
    void PlayerController::UpdatePlayerState(CharacterBody2D* body, float directionX)
    {
        // Hit State
        if (m_isHit)
        {
            m_info.set_state(::Protobuf::OBJECT_STATE_TYPE_HIT);
            return;
        }

        // Climbing State
        if (IsClimbing())
        {
            m_info.set_state(::Protobuf::OBJECT_STATE_TYPE_CLIMB);
            return;
        }

        // Above the ground : Jump
        if (!body->IsGrounded())
        {
            m_info.set_state(::Protobuf::OBJECT_STATE_TYPE_JUMP);
            return;
        }

        // Doesn't move on ground : Idle
        if (directionX == 0.0f)
        {
            m_info.set_state(::Protobuf::OBJECT_STATE_TYPE_IDLE);
            return;
        }

        // Running
        m_info.set_state
        (
            m_isRunning
            ? ::Protobuf::OBJECT_STATE_TYPE_RUN
            : ::Protobuf::OBJECT_STATE_TYPE_WALK
        );
    }
#pragma endregion
}
