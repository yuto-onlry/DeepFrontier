#pragma once

#include "DxLib.h"
#include "SphereCollider.h"

class CharacterBase;

class PlayerAction
{
public:
    enum class PlayerActionType
    {
        None,
        Attack,
        Avoid
    };

public:
    struct AttackData
    {
        int actionTime;
        int hitStartFrame;
        int hitEndFrame;
    };

private:
    PlayerActionType currentAction;

    int actionTimer;
    bool attackHit;

    // 攻撃のフレーム数
    int attackFrame;

    // 攻撃が当たるフレームの開始と終了
    int attackHitStartFrame;
    int attackHitEndFrame;

    SphereCollider attackCollider;

public:
    PlayerAction();
    ~PlayerAction();

public:
    void Init(CharacterBase* owner);

    void StartAttack(VECTOR playerPos, VECTOR forward);
    void StartAttack(VECTOR playerPos, VECTOR forward, const AttackData& attackData);

    void StartAvoid();

    void Update(VECTOR playerPos, VECTOR forward);

    void AttackCollider();
public:
    bool IsAction() const;
    bool IsAttack() const;

    bool IsComboAcceptFrame() const;
public:
    SphereCollider* GetAttackCollider();
};