#pragma once
#include "CharacterBase.h"
#include "AnimationManager.h"
#include "SphereCollider.h"

/// エネミーの状態
enum class EnemyState
{
    Idle,
    Chase,
    Wait,
    Attack,
    Damage,
    Dead
};

/// <summary>
/// エネミー基底クラス
/// </summary>
class EnemyBase : public CharacterBase
{
protected:
    // アニメーション管理
    AnimationManager animationManager;

    float moveSpeed;      // 移動速度
    float attackRange;    // 攻撃範囲

    // 攻撃判定用のコライダー
    SphereCollider attackCollider;

    bool isAttacking;
    bool isAttackHit;

    int attackTimer;
    int attackCoolTime;

    EnemyState state;

public:
    EnemyBase();
    virtual ~EnemyBase();

    void Init() override;

    // playerの座標と攻撃許可を受け取る
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
    // 初期化系
    void InitAttackCollider();

    // 状態更新系
    void Dead();
    void UpdateCoolTime();

    void DecideState(VECTOR playerPos, bool canAttack);

    void Idle();
    void Chase(VECTOR playerPos);
    void Wait(VECTOR playerPos);

    void StartAttack();
    void Attack(VECTOR playerPos);

    void AnimationAndCollider();

    // 判定系
    bool IsInAttackRange(VECTOR playerPos);
};