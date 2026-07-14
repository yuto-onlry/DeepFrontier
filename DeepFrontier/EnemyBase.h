#pragma once
#include "CharacterBase.h"
#include "AnimationManager.h"
#include "SphereCollider.h"
#include "EnemyReaction.h"

/// エネミーの状態
enum class EnemyState
{
    Idle,          // 待機
    Chase,         // 追跡
    Wait,          // プレイヤーとの距離を保つ
    Attack,        // 攻撃
    Damage,        // ダメージ

    KnockBack,     // 軽い吹っ飛び
    KnockbackDown, // 吹っ飛びダウン
    GetUp,         // 起き上がり

    Dead           // 死亡
};

/// <summary>
/// エネミー基底クラス
/// </summary>
class EnemyBase : public CharacterBase
{
protected:
    AnimationManager animationManager;

    float moveSpeed;
    float attackRange;

    SphereCollider attackCollider;

    bool isAttacking;
    bool isAttackHit;

    int attackTimer;
    int attackCoolTime;

    int waitMoveSign;
    int waitMoveTimer;

    int attackReserveCoolTime;

    EnemyState state;
private:
    EnemyReaction enemyReaction;


public:
    EnemyBase();
    virtual ~EnemyBase();

    void Init() override;

    virtual void Update(VECTOR playerPos, bool canAttack);

    void UpdateCollider();

    void Release() override;
    void Draw() override;

    void Damage(int damage) override;

    bool IsAttacking() const;

public:
    SphereCollider* GetAttackCollider();
    void DisableAttackCollider();

protected:
    void InitAttackCollider();

    void Dead();
    void UpdateCoolTime();

    void DecideState(VECTOR playerPos, bool canAttack);

    void Idle();
    void Chase(VECTOR playerPos);
    void Wait(VECTOR playerPos);

    void StartAttack();
    void Attack(VECTOR playerPos);
    void StopAttack();

    void AnimationAndCollider();
    void SetKnockDownRotation(VECTOR knockDir);

public:
    // 軽いノックバック
    void StartKnockBack(VECTOR direction, float power, int time);

    // 吹っ飛び + ダウン
    void StartKnockDown(
        VECTOR direction,
        float horizontalPower,
        float verticalPower,
        int knockbackDownTime,
        int getUpTime
    );

public:
    bool IsInAttackRange(VECTOR playerPos);
    bool CanAttack() const;
    bool IsHitReaction() const;
};