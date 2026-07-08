#include "LockOnManager.h"

LockOnManager::LockOnManager()
    : targetEnemy(nullptr),
    isLockOn(false),
    searchRange(700.0f)
{
}

LockOnManager::~LockOnManager()
{
}

void LockOnManager::Init()
{
    targetEnemy = nullptr;
    isLockOn = false;
    searchRange = 700.0f;
}

void LockOnManager::Update(
    const InputManager& inputManager,
    VECTOR playerPos,
    EnemyManager& enemyManager
)
{
    // 対象が消えていたら解除
    if (targetEnemy != nullptr)
    {
        if (enemyManager.ContainsEnemy(targetEnemy) == false)
        {
            Clear();
        }
        else if (targetEnemy->IsDead() == true)
        {
            Clear();
        }
    }

    // RBでロックオンON/OFF
    if (inputManager.IsButtonDown(InputManager::PadButton::RB))
    {
        if (isLockOn == true)
        {
            Clear();
            return;
        }

        targetEnemy = enemyManager.GetNearestEnemy(playerPos, searchRange);

        if (targetEnemy != nullptr)
        {
            isLockOn = true;
        }
    }
}

/// <summary>
/// ロックオンのデバック表示
/// </summary>
void LockOnManager::Draw()
{
    if (isLockOn == false || targetEnemy == nullptr)
    {
        return;
    }

    VECTOR targetPos = targetEnemy->GetPosition();

    VECTOR markerPos = VGet(
        targetPos.x,
        targetPos.y + 280.0f,
        targetPos.z
    );

    DrawSphere3D(
        markerPos,
        30.0f,
        16,
        GetColor(255, 0, 0),
        GetColor(255, 255, 255),
        FALSE
    );

    DrawLine3D(
        VGet(targetPos.x, targetPos.y + 180.0f, targetPos.z),
        markerPos,
        GetColor(255, 0, 0)
    );
}

/// <summary>
/// ロックオンを解除する
/// </summary>
void LockOnManager::Clear()
{
    targetEnemy = nullptr;
    isLockOn = false;
}
/// <summary>
/// ロックオン状態かどうかを取得する
/// </summary>
/// <returns></returns>
bool LockOnManager::IsLockOn() const
{
    return isLockOn;
}

/// <summary>
/// ロックオン対象の敵を取得する
/// </summary>
/// <returns></returns>
EnemyBase* LockOnManager::GetTargetEnemy() const
{
    return targetEnemy;
}

/// <summary>
/// ロックオン対象の敵の座標を取得する
/// </summary>
/// <returns></returns>
VECTOR LockOnManager::GetTargetPosition() const
{
    if (targetEnemy == nullptr)
    {
        return VGet(0.0f, 0.0f, 0.0f);
    }

    return targetEnemy->GetPosition();
}