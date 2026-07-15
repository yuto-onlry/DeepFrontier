#pragma once
#include "DxLib.h"
#include "Player.h"
#include "InputManager.h"
#include "CollisionManager.h"
#include "EnemyManager.h"
#include "LockOnManager.h"
#include "StageManager.h"
#include <memory>

class CharacterManager
{
private:
	// プレイヤーの状態を管理する    
    enum class BattlePhase
    {
        Field,
        Focus
    };
private:
    std::unique_ptr<Player> player;

    EnemyManager enemyManager;
    CollisionManager collisionManager;
    LockOnManager lockOnManager;
    BattlePhase battlePhase;
    EnemyBase* focusCandidateEnemy;
    EnemyBase* focusEnemy;

    int focusHitCount;
    int focusHitTimer;

public:
    CharacterManager();
    ~CharacterManager();
public:
    void Init();
    void Update(const InputManager& inputManager,VECTOR cameraForward,VECTOR cameraRight, const StageManager& stageManager);
    void Draw();
    void Release();

public:
    VECTOR GetPlayerPosition() const;
    VECTOR GetLockOnTargetPosition() const;
    Player* GetPlayer();
    EnemyBase* GetLockOnTarget() const;
    int GetEnemyCount() const;


    void SpawnWave(int waveNo);
private:
    void FocusPhase();
    void RegisterFocusHit(EnemyBase* hitEnemy);
    void StartFocus(EnemyBase* enemy);
    void EndFocus();
public:
    bool IsLockOn() const;
    bool IsAllEnemyDead() const;
    bool IsFocusPhase() const;
};