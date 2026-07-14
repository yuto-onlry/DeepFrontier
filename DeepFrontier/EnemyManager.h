#pragma once
#include "DxLib.h"
#include "EnemyBase.h"
#include "CollisionManager.h"
#include "Player.h"
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
	//指定されたEnemyが攻撃担当かどうかを判定する
    void SpawnWave(int waveNo);
	//最大同時攻撃可能数を取得する
    bool ContainsEnemy(EnemyBase* enemy) const;
	//すべての敵が死亡しているかどうかを判定する
    bool IsAllEnemyDead() const;
public:
    int GetEnemyCount() const;
	//指定されたEnemyが攻撃担当かどうかを判定する
    EnemyBase* GetNearestEnemy(VECTOR playerPos, float searchRange) const;
    void ResolvePlayer(Player* player);
public:
    void SetMaxEnemyAttackCount(int count);

private:
    void ResolveEnemyCollision();
	EnemyBase* SelectAttackEnemy(VECTOR playerPos);
	bool ExistsEnemy(EnemyBase* enemy) const;
    bool IsAttackEnemy(EnemyBase* enemy) const;
    void CleanupAttackEnemies();
};