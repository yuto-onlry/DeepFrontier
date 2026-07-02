#include "EnemyBase.h"
#include "DxLib.h"
#include <cmath>

EnemyBase::EnemyBase()
{
}
EnemyBase::~EnemyBase()
{
}

void EnemyBase::Init()
{
    hp = 30;
    attack = 5;
    defense = 0;

    position = VGet(300.0f, 0.0f, 300.0f);
    velocity = VGet(0.0f, 0.0f, 0.0f);

    moveSpeed = 2.0f;
    attackRange = 80.0f;

    modelHandle = -1;
    isDead = false;
}
void EnemyBase::Update(VECTOR playerPos)
{
    if (isDead == true)
    {
        capsuleCollider.SetActive(false);
        return;
    }

    // 攻撃範囲内なら止まる
    if (GetAttackRange(playerPos) == true)
    {
        velocity = VGet(0.0f, 0.0f, 0.0f);
        animationManager.ChangeAnim(AnimationType::Idle);
        animationManager.Update();

        MV1SetPosition(modelHandle, position);
        UpdateCollider();

        return;
    }

    GetMovePlayerPos(playerPos);

    animationManager.ChangeAnim(AnimationType::Run);
    animationManager.Update();

    MV1SetPosition(modelHandle, position);
    UpdateCollider();
}
void EnemyBase::GetMovePlayerPos(VECTOR playerPos)
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

		//モデルの向きをプレイヤーの方向に合わせる
        float modelOffset = DX_PI_F;

        MV1SetRotationXYZ(
            modelHandle,
            VGet(0.0f, angleY + modelOffset, 0.0f)
        );
    }
}
void EnemyBase::Damage(int damage)
{
    if (isDead == true)
    {
        return;
    }

    hp -= damage;

    printfDx("Enemy Damage HP:%d\n", hp);

    if (hp <= 0)
    {
        hp = 0;
        isDead = true;
        capsuleCollider.SetActive(false);

        printfDx("Enemy Dead\n");
    }
}
bool EnemyBase::IsDead() const
{
    return isDead;
}
bool EnemyBase::GetAttackRange(VECTOR playerPos)
{
    float dx = playerPos.x - position.x;
    float dz = playerPos.z - position.z;

    float distance = std::sqrt(dx * dx + dz * dz);

    if (distance <= attackRange)
    {
        return true;
    }

    return false;
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
}
void EnemyBase::UpdateCollider()
{
    if (isDead == true)
    {
        capsuleCollider.SetActive(false);
        return;
    }

    capsuleCollider.SetPosition(    
        VGet(position.x, position.y + 150.0f, position.z)
    );
}
void EnemyBase::Release()
{
    CharacterBase::Release();
}