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
        return;
    }

    // 攻撃範囲内なら止まる
    if (AttackRange(playerPos) == true)
    {
        velocity = VGet(0.0f, 0.0f, 0.0f);
        return;
    }

    // 攻撃範囲外ならプレイヤーへ近づく
    MovePlayer(playerPos);
}

void EnemyBase::MovePlayer(VECTOR playerPosition)
{
    VECTOR direction;

    direction.x = playerPosition.x - position.x;
    direction.y = 0.0f;
    direction.z = playerPosition.z - position.z;

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
}

bool EnemyBase::AttackRange(VECTOR playerPos)
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
    else
    {
        // モデルがない間は赤い球体で仮表示
        DrawSphere3D(
            position,
            30.0f,
            16,
            GetColor(255, 0, 0),
            GetColor(255, 0, 0),
            TRUE
        );
    }
}