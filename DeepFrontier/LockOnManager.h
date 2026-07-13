#pragma once
#include "DxLib.h"
#include "InputManager.h"
#include "EnemyManager.h"
#include "EnemyBase.h"

class LockOnManager
{
private:
    EnemyBase* targetEnemy;
    bool isLockOn;

    float searchRange;

public:
    LockOnManager();
    ~LockOnManager();
public:
    void Init();

    void Update(const InputManager& inputManager, VECTOR playerPos, EnemyManager& enemyManager);
    void Draw();
    void Clear();

public:
    bool IsLockOn() const;
    void LockOn(EnemyBase* enemy);
    EnemyBase* GetTargetEnemy() const;
    VECTOR GetTargetPosition() const;
private:
	bool IsTargetEnemy(EnemyBase* enemy, VECTOR playerPos) const;
};  