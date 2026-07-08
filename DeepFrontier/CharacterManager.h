#pragma once
#include "DxLib.h"
#include "Player.h"
#include "InputManager.h"
#include "CollisionManager.h"
#include "EnemyManager.h"
#include "LockOnManager.h"
#include <memory>

class CharacterManager
{
private:
    std::unique_ptr<Player> player;

    EnemyManager enemyManager;
    CollisionManager collisionManager;
    LockOnManager lockOnManager;

public:
    CharacterManager();
    ~CharacterManager();
public:
    void Init();
    void Update(const InputManager& inputManager,VECTOR cameraForward,VECTOR cameraRight);

    void Draw();
    void Release();

public:
    VECTOR GetPlayerPosition() const;
    Player* GetPlayer();
    EnemyBase* GetLockOnTarget() const;
    int GetEnemyCount() const;


    void SpawnWave(int waveNo);
public:
    bool IsLockOn() const;
    bool IsAllEnemyDead() const;
};