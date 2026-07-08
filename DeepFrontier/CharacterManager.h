#pragma once
#include "DxLib.h"
#include "Player.h"
#include "EnemyBase.h"
#include "InputManager.h"
#include "CollisionManager.h"
#include "EnemyManager.h"
#include "LockOnManager.h"
#include <vector>
#include <memory>

class CharacterManager
{
private:
	// プレイヤーと敵の管理クラス
    std::unique_ptr<Player> player;

	EnemyManager enemyManager;  
    CollisionManager collisionManager;
    LockOnManager lockOnManager;

public:
    CharacterManager();
    ~CharacterManager();

    void Init();
    void Update(const InputManager& inputManager, VECTOR cameraForward, VECTOR cameraRight);
    void Draw();
    void Release();

    VECTOR GetPlayerPosition() const;
    Player* GetPlayer();
    int GetEnemyCount() const;
    EnemyBase* GetLockOnTarget() const;
public:
	// 敵同士の衝突判定
    void ResolveEnemyCollision();
    void AddLittleEnemy(VECTOR pos);
    void SpawnWave(int waveNo);

public:
    bool IsAllEnemyDead() const;
    bool IsLockOn() const;

};