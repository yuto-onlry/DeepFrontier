#include "EnemyBase.h"
#include "DxLib.h"
#include <cmath>

EnemyBase::EnemyBase()
    : moveSpeed(0.0f),
    attackRange(0.0f),
    isAttacking(false),
    isAttackHit(false),
    attackTimer(0),
    attackCoolTime(0),
    waitMoveSign(1),
    waitMoveTimer(0),
    attackReserveCoolTime(0),
    state(EnemyState::Idle),
    enemyReaction()
{
}

EnemyBase::~EnemyBase()
{
    Release();
}

/// <summary>
/// 初期値
/// </summary>
void EnemyBase::Init()
{
    hp = 0;
    attack = 0;
    defense = 0;

    position = VGet(300.0f, 0.0f, 300.0f);
    velocity = VGet(0.0f, 0.0f, 0.0f);

    moveSpeed = 5.0f;
    attackRange = 160.0f;

    attackReserveCoolTime = 0;

    enemyReaction.Init();

    modelHandle = -1;
    isDead = false;

    waitMoveSign = (GetRand(1) == 0) ? -1 : 1;
    waitMoveTimer = 60 + GetRand(60);

    state = EnemyState::Idle;
}

void EnemyBase::InitAttackCollider()
{
    isAttacking = false;
    isAttackHit = false;

    attackTimer = 0;
    attackCoolTime = 0;

    attackCollider.SetTag(ColliderTag::EnemyAttack);
    attackCollider.SetOwner(this);
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

    if (enemyReaction.IsActive() == true)
    {
        enemyReaction.Update(
            modelHandle,
            position,
            animationManager,
            attackCollider
        );

        UpdateCollider();

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

void EnemyBase::UpdateCollider()
{
    if (isDead == true)
    {
        capsuleCollider.SetActive(false);
        return;
    }

    VECTOR colliderPos = enemyReaction.GetBodyColliderPosition(
        modelHandle,
        position
    );

    capsuleCollider.SetPosition(colliderPos);
}

void EnemyBase::Attack(VECTOR playerPos)
{
    const int attackDuration = 30;
    int startFrame = attackDuration - attackTimer;

    VECTOR direction;
    direction.x = playerPos.x - position.x;
    direction.y = 0.0f;
    direction.z = playerPos.z - position.z;

    float length = std::sqrt(
        direction.x * direction.x +
        direction.z * direction.z
    );

    if (length > 0.0f)
    {
        direction.x /= length;
        direction.z /= length;
    }

    if (modelHandle != -1)
    {
        float angleY = atan2f(direction.x, direction.z);
        float modelOffset = DX_PI_F;

        MV1SetRotationXYZ(
            modelHandle,
            VGet(0.0f, angleY + modelOffset, 0.0f)
        );
    }

    VECTOR attackPos;
    attackPos.x = position.x + direction.x * 80.0f;
    attackPos.y = position.y + 150.0f;
    attackPos.z = position.z + direction.z * 80.0f;

    attackCollider.SetPosition(attackPos);

    const int hitStartFrame = 0;
    const int hitEndFrame = 6;

    if (startFrame >= hitStartFrame &&
        startFrame <= hitEndFrame &&
        isAttackHit == false)
    {
        attackCollider.SetActive(true);
    }
    else
    {
        attackCollider.SetActive(false);
    }

    attackTimer--;

    if (attackTimer <= 0)
    {
        isAttacking = false;
        attackCollider.SetActive(false);

        state = EnemyState::Idle;

        attackReserveCoolTime = 60;

        animationManager.SetSpeed(0.5f);
        animationManager.ChangeAnim(AnimationType::Idle);
    }
}

void EnemyBase::StopAttack()
{
    isAttacking = false;
    isAttackHit = false;

    attackTimer = 0;
    attackCollider.SetActive(false);
}

void EnemyBase::StartAttack()
{
    state = EnemyState::Attack;

    isAttacking = true;
    isAttackHit = false;

    attackTimer = 30;
    attackCoolTime = 30;

    velocity = VGet(0.0f, 0.0f, 0.0f);

    attackCollider.SetActive(false);

    animationManager.SetSpeed(1.0f);
    animationManager.ChangeAnim(AnimationType::Attack);
}

SphereCollider* EnemyBase::GetAttackCollider()
{
    return &attackCollider;
}

void EnemyBase::StartKnockBack(VECTOR direction, float power, int time)
{
    if (isDead == true || enemyReaction.IsActive() == true)
    {
        return;
    }

    StopAttack();
    SetKnockDownRotation(direction);
    enemyReaction.StartKnockBack(
        direction,
        power,
        time,
        animationManager
    );
}

void EnemyBase::StartKnockDown(VECTOR direction,float horizontalPower,float verticalPower,int knockbackDownTime,int getUpTime)
{
    if (isDead == true || enemyReaction.IsActive() == true)
    {
        return;
    }

    StopAttack();

    enemyReaction.StartKnockDown(
        modelHandle,
        position,
        direction,
        horizontalPower,
        verticalPower,
        knockbackDownTime,
        getUpTime,
        animationManager
    );
}


void EnemyBase::Damage(int damage)
{
    if (isDead == true)
    {
        return;
    }

    hp -= damage;

    if (hp <= 0)
    {
        hp = 0;
        isDead = true;
        state = EnemyState::Dead;

        capsuleCollider.SetActive(false);
        attackCollider.SetActive(false);
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

    if (attackReserveCoolTime > 0)
    {
        attackReserveCoolTime--;
    }
}

void EnemyBase::DecideState(VECTOR playerPos, bool canAttack)
{
    if (isDead == true)
    {
        state = EnemyState::Dead;
        return;
    }

    if (canAttack == false)
    {
        state = EnemyState::Wait;
        return;
    }

    if (IsInAttackRange(playerPos) == true)
    {
        if (attackCoolTime <= 0)
        {
            state = EnemyState::Attack;
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

    animationManager.SetSpeed(1.2f);
    animationManager.ChangeAnim(AnimationType::Run);
}

void EnemyBase::Wait(VECTOR playerPos)
{
    const float keepDistance = 450.0f;
    const float margin = 80.0f;

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

    VECTOR sideDir;
    sideDir.x = -dir.z * (float)waitMoveSign;
    sideDir.y = 0.0f;
    sideDir.z = dir.x * (float)waitMoveSign;

    waitMoveTimer--;

    if (waitMoveTimer <= 0)
    {
        waitMoveTimer = 90 + GetRand(60);

        if (GetRand(1) == 0)
        {
            waitMoveSign *= -1;
        }
    }

    float lateralSpeed = moveSpeed * 1.0f;
    float distanceAdjustSpeed = moveSpeed * 0.4f;

    velocity = VGet(0.0f, 0.0f, 0.0f);

    velocity.x += sideDir.x * lateralSpeed;
    velocity.z += sideDir.z * lateralSpeed;

    if (distance < keepDistance - margin)
    {
        velocity.x += dir.x * distanceAdjustSpeed;
        velocity.z += dir.z * distanceAdjustSpeed;
    }
    else if (distance > keepDistance + margin)
    {
        velocity.x += -dir.x * distanceAdjustSpeed;
        velocity.z += -dir.z * distanceAdjustSpeed;
    }

    float velocityLength = std::sqrt(
        velocity.x * velocity.x +
        velocity.z * velocity.z
    );

    if (velocityLength > 0.0f)
    {
        velocity.x /= velocityLength;
        velocity.z /= velocityLength;

        velocity.x *= moveSpeed * 0.8f;
        velocity.z *= moveSpeed * 0.8f;
    }

    position.x += velocity.x;
    position.z += velocity.z;

    animationManager.SetSpeed(0.5f);

    if (waitMoveSign < 0)
    {
        animationManager.ChangeAnim(AnimationType::MoveLeft);
    }
    else
    {
        animationManager.ChangeAnim(AnimationType::MoveRight);
    }

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


void EnemyBase::StartDamageReaction()
{
    if (isDead == true)
    {
        return;
    }

    if (enemyReaction.IsActive() == true)
    {
        return;
    }

    StopAttack();

    enemyReaction.StartDamage(
        20,
        animationManager
    );
}


void EnemyBase::AnimationAndCollider()
{
    animationManager.Update();

    if (modelHandle != -1)
    {
        MV1SetPosition(modelHandle, position);
    }

    UpdateCollider();
}

void EnemyBase::SetKnockDownRotation(VECTOR knockDir)
{
    knockDir.y = 0.0f;

    float length = sqrtf(
        knockDir.x * knockDir.x +
        knockDir.z * knockDir.z
    );

    if (length <= 0.001f)
    {
        return;
    }

    knockDir.x /= length;
    knockDir.z /= length;

    // knockDir は「プレイヤーから敵が離れる方向」
    // 頭がプレイヤー側に来るなら、モデルの向きを反転させる
    VECTOR faceDir;

    faceDir.x = knockDir.x;
    faceDir.y = 0.0f;
    faceDir.z = knockDir.z;

    float angleY = atan2f(faceDir.x, faceDir.z);

    // Enemyモデルで普段使っている補正
    float modelOffset = DX_PI_F;

    if (modelHandle != -1)
    {
        MV1SetRotationXYZ(
            modelHandle,
            VGet(0.0f, angleY + modelOffset, 0.0f)
        );
    }
}

/// <summary>
/// 
/// </summary>
/// <param name="newPosition"></param>
void EnemyBase::SetPositionForCollision(VECTOR newPosition)
{
    position = newPosition;

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
}

void EnemyBase::DisableAttackCollider()
{
    isAttackHit = true;
    attackCollider.SetActive(false);
}

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

void EnemyBase::Release()
{
    attackCollider.SetActive(false);

    animationManager.Release();

    CharacterBase::Release();
}

bool EnemyBase::IsAttacking() const
{
    return isAttacking;
}

bool EnemyBase::CanAttack() const
{
    if (isDead == true)
    {
        return false;
    }

    if (isAttacking == true)
    {
        return true;
    }

    return attackReserveCoolTime <= 0;
}

bool EnemyBase::IsHitReaction() const
{
    return enemyReaction.IsActive();
}