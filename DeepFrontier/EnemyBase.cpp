#include "EnemyBase.h"
#include "DxLib.h"
#include <cmath>

EnemyBase::EnemyBase()
{
}
EnemyBase::~EnemyBase()
{
    Release();
}

void EnemyBase::Init()
{
    hp = 0;
    attack = 0;
    defense = 0;

    position = VGet(300.0f, 0.0f, 300.0f);
    velocity = VGet(0.0f, 0.0f, 0.0f);

    moveSpeed = 5.0f;
    attackRange = 80.0f;

    modelHandle = -1;
    isDead = false;
    state = EnemyState::Idle;
}
/// <summary>
/// 攻撃判定の初期化
/// </summary>
void EnemyBase::InitAttackCollider()
{
    isAttacking = false;
    isAttackHit = false;

    attackTimer = 0;
    attackCoolTime = 0;

    attackCollider.SetTag(ColliderTag::EnemyAttack);
    attackCollider.SetOwner(this);
	// 攻撃判定の大きさ
    attackCollider.SetRadius(55.0f);
    attackCollider.SetActive(false);
}
void EnemyBase::Update(VECTOR playerPos, bool canAttack)
{
    if (isDead == true)
    {
        state = EnemyState::Dead;
        Dead();
        return;
    }

    UpdateCoolTime();

    if (state == EnemyState::Attack)
    {
        Attack(playerPos);
        AnimationAndCollider();
        return;
    }

    DecideState(playerPos, canAttack);

    switch (state)
    {
    case EnemyState::Idle:
        Idle();
        break;

    case EnemyState::Chase:
        Chase(playerPos);
        break;

    case EnemyState::Wait:
        Wait(playerPos);
        break;

    case EnemyState::Attack:
        StartAttack();
        break;

    case EnemyState::Dead:
        Dead();
        break;

    default:
        break;
    }

    AnimationAndCollider();
}
/// <summary>
/// 攻撃判定の更新
/// </summary>
void EnemyBase::UpdateCollider()
{
    if (isDead == true)
    {
        capsuleCollider.SetActive(false);
        return;
    }
	// エネミーコライダーの位置を更新
    capsuleCollider.SetPosition(VGet(position.x, position.y + 150.0f, position.z));
}
/// <summary>
/// 攻撃中の更新
/// </summary>
/// <param name="playerPos"></param>
void EnemyBase::Attack(VECTOR playerPos)
{
	// 攻撃アニメーションのフレーム数
    const int attackDuration = 30;
	// 攻撃アニメーションの経過フレーム数
    int startFrame = attackDuration - attackTimer;

    VECTOR direction;
    direction.x = playerPos.x - position.x;
    direction.y = 0.0f;
    direction.z = playerPos.z - position.z;

    float length = std::sqrt(direction.x * direction.x + direction.z * direction.z);

    if (length > 0.0f)
    {
        direction.x /= length;
        direction.z /= length;
    }

    if (modelHandle != -1)
    {
        float angleY = atan2f(direction.x, direction.z);
        float modelOffset = DX_PI_F;
		// 攻撃中はプレイヤーの方向を向く
        MV1SetRotationXYZ(modelHandle, VGet(0.0f, angleY + modelOffset, 0.0f));
    }

    VECTOR attackPos;
    attackPos.x = position.x + direction.x * 80.0f;
    attackPos.y = position.y + 150.0f;
    attackPos.z = position.z + direction.z * 80.0f;
    attackCollider.SetPosition(attackPos);
	// 攻撃判定の有効化フレーム範囲
    const int hitStartFrame = 0;
	// 攻撃判定の無効化フレーム範囲
    const int hitEndFrame = 6;
	// 攻撃判定の有効化・無効化
    if (startFrame >= hitStartFrame && startFrame <= hitEndFrame && isAttackHit == false)
        attackCollider.SetActive(true);
    else
        attackCollider.SetActive(false);

    attackTimer--;
	// 攻撃アニメーション終了後の処理
    if (attackTimer <= 0)
    {
        isAttacking = false;
        attackCollider.SetActive(false);
        state = EnemyState::Idle;
		// 攻撃後のクールタイムを設定
        animationManager.SetSpeed(0.5f);
        animationManager.ChangeAnim(AnimationType::Idle);
    }
}
/// <summary>
/// 攻撃開始
/// </summary>
void EnemyBase::StartAttack()
{
    state = EnemyState::Attack;
    isAttacking = true;
    isAttackHit = false;
	// 攻撃タイマーとクールタイムをリセット
    attackTimer = 30;
	// 攻撃後のクールタイムを設定
    attackCoolTime = 30;
	// 攻撃中は移動しないように速度をリセット
    velocity = VGet(0.0f, 0.0f, 0.0f);
	// 攻撃判定を無効化
    attackCollider.SetActive(false);
	// 攻撃アニメーションを再生
    animationManager.SetSpeed(1.0f);
    animationManager.ChangeAnim(AnimationType::Attack);
}
/// <summary>
/// 攻撃判定のコライダーを取得
/// </summary>
SphereCollider* EnemyBase::GetAttackCollider()
{
    return &attackCollider;
}
/// <summary>
/// ダメージ処理
/// </summary>
/// <param name="damage"></param>
void EnemyBase::Damage(int damage)
{
    if (isDead == true)
        return;

    hp -= damage;

    if (hp <= 0)
    {
        hp = 0;
        isDead = true;
        state = EnemyState::Dead;
        capsuleCollider.SetActive(false);
    }
}
void EnemyBase::Dead()
{
    velocity = VGet(0.0f, 0.0f, 0.0f);

    capsuleCollider.SetActive(false);
    attackCollider.SetActive(false);

    isAttacking = false;
}

void EnemyBase::UpdateCoolTime()
{
    if (attackCoolTime > 0)
    {
        attackCoolTime--;
    }
}

void EnemyBase::DecideState(VECTOR playerPos, bool canAttack)
{
    if (isDead == true)
    {
        state = EnemyState::Dead;
        return;
    }

    if (IsInAttackRange(playerPos) == true)
    {
        if (canAttack == true && attackCoolTime <= 0)
        {
            state = EnemyState::Attack;
        }
        else if (canAttack == false)
        {
            state = EnemyState::Wait;
        }
        else
        {
            state = EnemyState::Idle;
        }

        return;
    }

    state = EnemyState::Chase;
}

void EnemyBase::Idle()
{
    velocity = VGet(0.0f, 0.0f, 0.0f);

    animationManager.SetSpeed(0.5f);
    animationManager.ChangeAnim(AnimationType::Idle);
}

void EnemyBase::Chase(VECTOR playerPos)
{
    VECTOR direction;

    direction.x = playerPos.x - position.x;
    direction.y = 0.0f;
    direction.z = playerPos.z - position.z;

    float length = std::sqrt(
        direction.x * direction.x +
        direction.z * direction.z
    );

    if (length <= 0.0f)
    {
        velocity = VGet(0.0f, 0.0f, 0.0f);
        return;
    }

    direction.x /= length;
    direction.z /= length;

    velocity.x = direction.x * moveSpeed;
    velocity.y = 0.0f;
    velocity.z = direction.z * moveSpeed;

    position = VAdd(position, velocity);

    if (modelHandle != -1)
    {
        float angleY = atan2f(velocity.x, velocity.z);
        float modelOffset = DX_PI_F;

        MV1SetRotationXYZ(
            modelHandle,
            VGet(0.0f, angleY + modelOffset, 0.0f)
        );
    }

    animationManager.SetSpeed(0.5f);
    animationManager.ChangeAnim(AnimationType::Run);
}

void EnemyBase::Wait(VECTOR playerPos)
{
    const float keepDistance = 180.0f;
    const float margin = 20.0f;

    float dx = position.x - playerPos.x;
    float dz = position.z - playerPos.z;

    float distanceSq = dx * dx + dz * dz;

    if (distanceSq <= 0.0001f)
    {
        dx = 1.0f;
        dz = 0.0f;
        distanceSq = 1.0f;
    }

    float distance = std::sqrt(distanceSq);

    VECTOR dir;
    dir.x = dx / distance;
    dir.y = 0.0f;
    dir.z = dz / distance;

    velocity = VGet(0.0f, 0.0f, 0.0f);

    // プレイヤーに近すぎる場合は離れる
    if (distance < keepDistance - margin)
    {
        velocity.x = dir.x * moveSpeed;
        velocity.z = dir.z * moveSpeed;

        position.x += velocity.x;
        position.z += velocity.z;

        animationManager.SetSpeed(0.5f);
        animationManager.ChangeAnim(AnimationType::Run);
    }
    // 離れすぎている場合は少し近づく
    else if (distance > keepDistance + margin)
    {
        velocity.x = -dir.x * moveSpeed;
        velocity.z = -dir.z * moveSpeed;

        position.x += velocity.x;
        position.z += velocity.z;

        animationManager.SetSpeed(0.5f);
        animationManager.ChangeAnim(AnimationType::Run);
    }
    // ちょうどいい距離なら待機
    else
    {
        animationManager.SetSpeed(0.5f);
        animationManager.ChangeAnim(AnimationType::Idle);
    }

    // プレイヤーの方向を見る
    if (modelHandle != -1)
    {
        float angleY = atan2f(-dir.x, -dir.z);
        float modelOffset = DX_PI_F;

        MV1SetRotationXYZ(
            modelHandle,
            VGet(0.0f, angleY + modelOffset, 0.0f)
        );
    }
}
/// <summary>
/// アニメーションとコライダーの更新
/// </summary>
void EnemyBase::AnimationAndCollider()
{
    animationManager.Update();

    if (modelHandle != -1)
    {
        MV1SetPosition(modelHandle, position);
    }

    UpdateCollider();
}

bool EnemyBase::IsInAttackRange(VECTOR playerPos)
{
    float dx = playerPos.x - position.x;
    float dz = playerPos.z - position.z;

    float distance = std::sqrt(dx * dx + dz * dz);

    return distance <= attackRange;
}bool EnemyBase::IsAttacking() const
{
    return isAttacking;
}
/// <summary>
/// 攻撃判定のコライダーを無効化
/// </summary>
void EnemyBase::DisableAttackCollider()
{
    isAttackHit = true;
    attackCollider.SetActive(false);
}
/// <summary>
/// 描画
/// </summary>
void EnemyBase::Draw()
{
    if (isDead == true)
    {
        return;
    }

    if (modelHandle != -1)
    {
        CharacterBase::Draw();
    }

    attackCollider.DrawDebug();
}
/// <summary>
/// リソース解放
/// </summary>
void EnemyBase::Release()
{
    attackCollider.SetActive(false);
	//攻撃判定のコライダーを解放
    animationManager.Release();
	//モデルを解放
    CharacterBase::Release();   
}