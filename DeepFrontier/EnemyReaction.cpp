#include "EnemyReaction.h"
#include <cmath>

EnemyReaction::EnemyReaction()
    : state(ReactionState::None),
    knockBackVelocity(VGet(0.0f, 0.0f, 0.0f)),
    knockBackTimer(0),
    knockbackDownTimer(0),
    getUpTimer(0),
    gravity(0.8f),
    isGroundedAfterKnockBack(false),
    rootMotion(),
    damageTimer(0)
{
}

void EnemyReaction::Init()
{
    state = ReactionState::None;

    knockBackVelocity = VGet(0.0f, 0.0f, 0.0f);

    knockBackTimer = 0;
    knockbackDownTimer = 0;
    getUpTimer = 0;
    damageTimer = 0;

    gravity = 0;

    isGroundedAfterKnockBack = false;

    rootMotion.Init();
}

void EnemyReaction::StartKnockBack(VECTOR direction, float power,int time,AnimationManager& animationManager)
{
    if (IsActive() == true)
    {
        return;
    }

    direction.y = 0.0f;

    float length = sqrtf(
        direction.x * direction.x +
        direction.z * direction.z
    );

    if (length <= 0.001f)
    {
        return;
    }

    direction.x /= length;
    direction.z /= length;

    state = ReactionState::KnockBack;

    knockBackTimer = time;

    knockBackVelocity.x = direction.x * power;
    knockBackVelocity.y = 0.0f;
    knockBackVelocity.z = direction.z * power;

    animationManager.SetSpeed(1.5f);
    animationManager.ChangeAnim(AnimationType::Damage);
}

void EnemyReaction::StartKnockDown(
    int modelHandle,
    VECTOR position,
    VECTOR direction,
    float horizontalPower,
    float verticalPower,
    int knockbackDownTime,
    int getUpTime,
    AnimationManager& animationManager
)
{
    if (IsActive() == true)
    {
        return;
    }

    direction.y = 0.0f;

    float length = sqrtf(
        direction.x * direction.x +
        direction.z * direction.z
    );

    if (length <= 0.001f)
    {
        return;
    }

    direction.x /= length;
    direction.z /= length;

    state = ReactionState::KnockbackDown;

    knockBackVelocity.x = direction.x * horizontalPower;
    knockBackVelocity.y = verticalPower;
    knockBackVelocity.z = direction.z * horizontalPower;

    knockbackDownTimer = knockbackDownTime;
    getUpTimer = getUpTime;

    isGroundedAfterKnockBack = false;

    if (modelHandle != -1)
    {
        MV1SetPosition(modelHandle, position);
    }

    animationManager.SetSpeed(1.0f);
    animationManager.ChangeAnim(AnimationType::KnockbackDown);

    // KnockbackDown開始時のHipsズレを保存
    rootMotion.Start(modelHandle, position);
}

void EnemyReaction::Update(int modelHandle,VECTOR& position,AnimationManager& animationManager,SphereCollider& attackCollider)
{
    if (state == ReactionState::Damage)
    {
        Damage(modelHandle, position, animationManager, attackCollider);
        return;
    }
    if (state == ReactionState::KnockBack)
    {
        KnockBack(
            modelHandle,
            position,
            animationManager,
            attackCollider
        );

        return;
    }

    if (state == ReactionState::KnockbackDown)
    {
        KnockbackDown(
            modelHandle,
            position,
            animationManager,
            attackCollider
        );

        return;
    }

    if (state == ReactionState::GetUp)
    {
        GetUp(
            modelHandle,
            position,
            animationManager,
            attackCollider
        );

        return;
    }
}

void EnemyReaction::KnockBack(
    int modelHandle,
    VECTOR& position,
    AnimationManager& animationManager,
    SphereCollider& attackCollider
)
{
    position = VAdd(position, knockBackVelocity);

    knockBackVelocity.x *= 0.90f;
    knockBackVelocity.z *= 0.90f;

    knockBackTimer--;

    if (knockBackTimer <= 0)
    {
        state = ReactionState::None;

        knockBackVelocity = VGet(0.0f, 0.0f, 0.0f);

        animationManager.SetSpeed(0.5f);
        animationManager.ChangeAnim(AnimationType::Idle);
    }

    attackCollider.SetActive(false);

    animationManager.Update();

    if (modelHandle != -1)
    {
        MV1SetPosition(modelHandle, position);
    }
}
void EnemyReaction::KnockbackDown(
    int modelHandle,
    VECTOR& position,
    AnimationManager& animationManager,
    SphereCollider& attackCollider
)
{
    attackCollider.SetActive(false);

    if (isGroundedAfterKnockBack == false)
    {
        position = VAdd(position, knockBackVelocity);

        knockBackVelocity.x *= 0.94f;
        knockBackVelocity.z *=0.94f;

        knockBackVelocity.y -= gravity;

        if (position.y <= 0.0f && knockBackVelocity.y <= 0.0f)
        {
            position.y = 0.0f;

            knockBackVelocity = VGet(0.0f, 0.0f, 0.0f);
            isGroundedAfterKnockBack = true;
        }
    }

    knockbackDownTimer--;

    animationManager.Update();

    if (modelHandle != -1)
    {
        MV1SetPosition(modelHandle, position);
    }
     
    if (knockbackDownTimer <= 0)
    {
        
        rootMotion.AbsorbToPosition(modelHandle, position);

        state = ReactionState::GetUp;
        //アニメーション再生スピード
        animationManager.SetSpeed(0.4f);
        animationManager.ChangeAnim(AnimationType::GetUp);

        rootMotion.Start(modelHandle, position);
    }
}

void EnemyReaction::GetUp(
    int modelHandle,
    VECTOR& position,
    AnimationManager& animationManager,
    SphereCollider& attackCollider
)
{
    attackCollider.SetActive(false);

    getUpTimer--;

    animationManager.Update();

    if (modelHandle != -1)
    {
        MV1SetPosition(modelHandle, position);
    }

    if (getUpTimer <= 0)
    {
        // GetUpの最後のHips位置をpositionに吸収
        rootMotion.AbsorbToPosition(modelHandle, position);

        state = ReactionState::None;

        animationManager.SetSpeed(0.5f);
        animationManager.ChangeAnim(AnimationType::Idle);
    }
}

void EnemyReaction::StartDamage(int time,AnimationManager& animationManager)
{
    if (IsActive() == true)
    {
        return;
    }

    state = ReactionState::Damage;
    damageTimer = time;

    animationManager.SetSpeed(1.0f);
    animationManager.ChangeAnim(AnimationType::Damage);
}

void EnemyReaction::Damage(int modelHandle, VECTOR& position,AnimationManager& animationManager,SphereCollider& attackCollider)
{
    attackCollider.SetActive(false);

    damageTimer--;

    if (damageTimer <= 0)
    {
        state = ReactionState::None;

        animationManager.SetSpeed(0.5f);
        animationManager.ChangeAnim(AnimationType::Idle);
    }

    animationManager.Update();

    if (modelHandle != -1)
    {
        MV1SetPosition(modelHandle, position);
    }
}
bool EnemyReaction::IsActive() const
{
    return state != ReactionState::None;
}

bool EnemyReaction::IsKnockBack() const
{
    return state == ReactionState::KnockBack;
}

bool EnemyReaction::IsKnockbackDown() const
{
    return state == ReactionState::KnockbackDown;
}

bool EnemyReaction::IsGetUp() const
{
    return state == ReactionState::GetUp;
}

VECTOR EnemyReaction::GetBodyColliderPosition(
    int modelHandle,
    VECTOR position
)
{
    bool useHipsXZ =
        state == ReactionState::KnockbackDown ||
        state == ReactionState::GetUp;

    return rootMotion.GetBodyColliderPosition(
        modelHandle,
        position,
        useHipsXZ
    );
}