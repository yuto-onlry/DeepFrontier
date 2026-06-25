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

    // 敵の初期化＋配置
    AddLittleEnemy(VGet(300.0f, 0.0f, 300.0f));
    AddLittleEnemy(VGet(100.0f, 0.0f, 100.0f));
    AddLittleEnemy(VGet(200.0f, 0.0f, 200.0f));
    AddLittleEnemy(VGet(400.0f, 0.0f, 400.0f));
}
void CharacterManager::Update(const InputManager& inputManager)
{
    if (player != nullptr)
    {
        player->Update(inputManager);
    }

    VECTOR playerPos = GetPlayerPosition();

    for (auto& enemy : enemies)
    {
        enemy->Update(playerPos);
    }

    collisionManager.Clear();

    if (player != nullptr)
    {
        collisionManager.AddCollider(player->GetCapsuleCollider());
        collisionManager.AddCollider(player->GetAttackCollider());
    }

    for (auto& enemy : enemies)
    {
        collisionManager.AddCollider(enemy->GetCapsuleCollider());
    }
    collisionManager.CheckCollision();
    const auto& hits = collisionManager.GetCollisionHit();
    for (const auto& hit : hits)
    {
        ColliderBase* a = hit.colliderA;
        ColliderBase* b = hit.colliderB;

        if (a == nullptr || b == nullptr)
        {
            continue;
        }

        ColliderBase* attackCollider = nullptr;
        ColliderBase* enemyCollider = nullptr;

        if (a->GetTag() == ColliderTag::PlayerAttack &&
            b->GetTag() == ColliderTag::Enemy)
        {
            attackCollider = a;
            enemyCollider = b;
        }
        else if (a->GetTag() == ColliderTag::Enemy &&
            b->GetTag() == ColliderTag::PlayerAttack)
        {
            attackCollider = b;
            enemyCollider = a;
        }

        if (attackCollider != nullptr && enemyCollider != nullptr)
        {
            CharacterBase* enemy = enemyCollider->GetOwner();

            if (enemy != nullptr)
            {
                enemy->Damage(999);

                if (player != nullptr)
                {
                    player->DisableAttackCollider();
                }
                collisionManager.Clear();
                break;
            }
        }
    }

    // 死亡した敵を削除
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        if ((*it)->IsDead() == true)
        {
            (*it)->Release();
            it = enemies.erase(it);
        }
        else {
            ++it;
        }
    }
}
void CharacterManager::Draw()
{
    if (player != nullptr)
        player->Draw();
    for (auto& enemy : enemies)
        enemy->Draw();
	// デバッグ用のコライダー描画    
    collisionManager.DrawDebug();
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

// 敵の生成関数(弱)
void CharacterManager::AddLittleEnemy(VECTOR pos)
{
    std::unique_ptr<EnemyBase> enemy = std::make_unique<LittleEnemy>();

    enemy->Init();
    enemy->SetPosition(pos);

    enemies.push_back(std::move(enemy));
}