#pragma once
#include "DxLib.h"
#include "Player.h"
#include "EnemyBase.h"
#include "InputManager.h"
#include <vector>
#include <memory>

class CharacterManager
{
private:
	// プレイヤーと敵の管理クラス
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<EnemyBase>> enemies;

public:
    CharacterManager();
    ~CharacterManager();

    void Init();
    void Update(const InputManager& inputManager);
    void Draw();
    void Release();

    VECTOR GetPlayerPosition() const;
    Player* GetPlayer();

    void AddLittleEnemy(VECTOR pos);
};