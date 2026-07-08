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
	// 最大同時攻撃可能数
    int maxEnemyAttackCount;
    // 攻撃担当のEnemy
    std::vector<EnemyBase*> attackEnemies;
public:
    EnemyManager();
    ~EnemyManager();
public:
    void Init();
    void Update(VECTOR playerPos);
    void Draw();
    void Release();

    void AddLittleEnemy(VECTOR pos);
    void ClearEnemies();
    void RemoveDeadEnemies();

    void AddColliders(CollisionManager& collisionManager);
    bool IsAllEnemyDead() const;
    void SpawnWave(int waveNo);
public:
    int GetEnemyCount() const;
public:
    void SetMaxEnemyAttackCount(int count);

private:
    void ResolveEnemyCollision();
	EnemyBase* SelectAttackEnemy(VECTOR playerPos);
	bool ExistsEnemy(EnemyBase* enemy) const;
    bool IsAttackEnemy(EnemyBase* enemy) const;
    void CleanupAttackEnemies();
};