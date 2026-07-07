#pragma once
#include "DxLib.h"
#include "Player.h"
#include "EnemyBase.h"
#include "InputManager.h"
#include "CollisionManager.h"
#include <vector>
#include <memory>

class CharacterManager
{
private:
	// プレイヤーと敵の管理クラス
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<EnemyBase>> enemies;
    CollisionManager collisionManager;

public:
    CharacterManager();
    ~CharacterManager();

    void Init();
    void Update(const InputManager& inputManager, VECTOR cameraForward, VECTOR cameraRight);
    void Draw();
    void Release();

    VECTOR GetPlayerPosition() const;
    Player* GetPlayer();
private:
	// 敵同士の衝突判定
    void ResolveEnemyCollision();

    void AddLittleEnemy(VECTOR pos);
};