#pragma once
#include "CharacterBase.h"
#include "AnimationManager.h"
#include "SphereCollider.h"

/// エネミーの状態
enum class EnemyState
{
	Idle,   // 待機
	Chase,  // 追跡
	Wait,   // プレイヤーとの距離を保つ
	Attack, // 攻撃
	Damage, // ダメージ
	Dead    // 死亡
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
	// 攻撃中かどうかのフラグ
    bool isAttacking;
    bool isAttackHit;
	// 攻撃タイマーとクールタイム
    int attackTimer;
    int attackCoolTime;
	// プレイヤーとの距離を保つための待機タイマー
    int waitMoveSign;
    int waitMoveTimer;
	int attackReserveCoolTime;  // 攻撃予約のクールタイム
	// エネミーの状態
    EnemyState state;
private:
    bool isKnockBack;
    VECTOR knockBackVelocity;
    int knockBackTimer;

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
public:
    void StartKnockBack(VECTOR direction, float power, int time);

public:
    // 判定系
    bool IsInAttackRange(VECTOR playerPos);
	bool CanAttack() const;
    bool IsKnockBack() const;
};