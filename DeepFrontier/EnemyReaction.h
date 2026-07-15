#pragma once
#include "DxLib.h"
#include "AnimationManager.h"
#include "SphereCollider.h"
#include "EnemyRootMotion.h"

class EnemyReaction
{
public:
    enum class ReactionState
    {
        None,
        KnockBack,
        KnockbackDown,
        Damage,
        GetUp
    };

private:
    ReactionState state;

    VECTOR knockBackVelocity;

    int knockBackTimer;
    int knockbackDownTimer;
    int getUpTimer;
    int damageTimer;

    float gravity;

    bool isGroundedAfterKnockBack;

    EnemyRootMotion rootMotion;

public:
    EnemyReaction();

    void Init();

    void StartKnockBack(VECTOR direction,float power,int time,AnimationManager& animationManager);

    void StartKnockDown(int modelHandle,
        VECTOR position, VECTOR direction,
        float horizontalPower,float verticalPower,
        int knockbackDownTime,int getUpTime,
        AnimationManager& animationManager
    );
    void StartDamage(int timer, AnimationManager& animationManager);
    void Update(int modelHandle,VECTOR& position,AnimationManager& animationManager,SphereCollider& attackCollider);

    bool IsActive() const;
    bool IsKnockBack() const;
    bool IsKnockbackDown() const;
    bool IsGetUp() const;

    VECTOR GetBodyColliderPosition(
        int modelHandle,
        VECTOR position
    );

private:
    void KnockBack(
        int modelHandle,
        VECTOR& position,
        AnimationManager& animationManager,
        SphereCollider& attackCollider
    );

    void KnockbackDown(
        int modelHandle,
        VECTOR& position,
        AnimationManager& animationManager,
        SphereCollider& attackCollider
    );

    void GetUp(
        int modelHandle,
        VECTOR& position,
        AnimationManager& animationManager,
        SphereCollider& attackCollider
    );

    void Damage(int modelHandle, VECTOR& position, AnimationManager& animationManager, SphereCollider& attackCollider);

};