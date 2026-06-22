#include "CharacterBase.h"
#include "CharacterManager.h"
#include "LittleEnemy.h"

CharacterManager::CharacterManager()
{
}

CharacterManager::~CharacterManager()
{
    Release();
}

void CharacterManager::Init()
{
    player = std::make_unique<Player>();
    player->Init();

	//敵の初期化＋配置
    AddLittleEnemy(VGet(300.0f, 0.0f, 300.0f));
}
void CharacterManager::Update(const InputManager& inputManager)
{
    VECTOR playerPos = GetPlayerPosition();
    if (player != nullptr)
        player->Update(inputManager);

    for (auto& enemy : enemies)
        enemy->Update(playerPos);
    // 死亡した敵を削除
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        if ((*it)->IsDead() == true)
        {
            it = enemies.erase(it);
        }else
        {
            ++it;
        }
    }
}

void CharacterManager::Draw()
{
    if (player != nullptr)
    {
        player->Draw();
    }
    for (auto& enemy : enemies)
    {
        enemy->Draw();
    }
}

void CharacterManager::Release()
{
    if (player != nullptr)
    {
        player->Release();
        player.reset();
    }
    for (auto& enemy : enemies)
    {
        enemy->Release();
    }

    enemies.clear();
}

VECTOR CharacterManager::GetPlayerPosition() const
{
    if (player != nullptr)
    {
        return player->GetPosition();
    }

    return VGet(0.0f, 0.0f, 0.0f);
}

Player* CharacterManager::GetPlayer()
{
    return player.get();
}

// 弱い敵を追加する関数
void CharacterManager::AddLittleEnemy(VECTOR pos)
{
    std::unique_ptr<EnemyBase> enemy = std::make_unique<LittleEnemy>();

    enemy->Init();
    enemy->SetPosition(pos);

    enemies.push_back(std::move(enemy));
}