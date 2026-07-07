#pragma once
#include "DxLib.h"
#include "EnemyBase.h"
#include "CollisionManager.h"
#include <vector>
#include <memory>

class EnemyManager
{
private:
    std::vector<std::unique_ptr<EnemyBase>> enemies;

    int maxEnemyAttackCount;

public:
    EnemyManager();
    ~EnemyManager();

    void Init();
    void Update(VECTOR playerPos);
    void Draw();
    void Release();

    void AddLittleEnemy(VECTOR pos);
    void ClearEnemies();
    void RemoveDeadEnemies();

    bool IsAllEnemyDead() const;
    int GetEnemyCount() const;

    void AddColliders(CollisionManager& collisionManager);

    void SpawnWave(int waveNo);

private:
    void ResolveEnemyCollision();
};