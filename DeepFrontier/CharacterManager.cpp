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
    AddLittleEnemy(VGet(-100.0f, 0.0f, 100.0f));
    AddLittleEnemy(VGet(-200.0f, 0.0f, -200.0f));
    AddLittleEnemy(VGet(400.0f, 0.0f, -400.0f));
}
void CharacterManager::Update(const InputManager& inputManager,VECTOR cameraForward,VECTOR cameraRight)
{
    collisionManager.Clear();

    // 前フレームまでに死亡した敵を削除
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        if ((*it)->IsDead() == true)
        {
            (*it)->Release();
            it = enemies.erase(it);
        }
        else
        {
            ++it;
        }
    }
    if (player != nullptr)
    {
        player->Update(inputManager, cameraForward, cameraRight);
    }

    VECTOR playerPos = GetPlayerPosition();

    for (auto& enemy : enemies)
    {
        enemy->Update(playerPos);
    }
    ResolveEnemyCollision();
    collisionManager.Clear();

    if (player != nullptr)
    {
        collisionManager.AddCollider(player->GetCapsuleCollider());
        for (int i = 0; i < player->GetAttackColliderCount(); i++)
        {
            collisionManager.AddCollider(player->GetAttackCollider(i));
        }
    }

    for (auto& enemy : enemies)
    {
		// 敵が生存している場合のみコライダーを追加
        if (enemy != nullptr && enemy->IsDead() == false)
        {
            collisionManager.AddCollider(enemy->GetCapsuleCollider());
            collisionManager.AddCollider(enemy->GetAttackCollider());
        }
    }
    collisionManager.CheckCollision();
    const auto& hits = collisionManager.GetCollisionHit();
	// 衝突判定の処理
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
		// 攻撃判定と敵のコライダーが衝突した場合の処理
        if (attackCollider != nullptr && enemyCollider != nullptr)
        {
            CharacterBase* enemy = enemyCollider->GetOwner();

            if (enemy != nullptr && player != nullptr)
            {
                enemy->Damage(player->GetAttack());
                player->DisableAttackCollider();
                break;
            }
        }
		// 敵の攻撃判定とプレイヤーのコライダーが衝突した場合の処理
        ColliderBase* enemyAttackCollider = nullptr;
        ColliderBase* playerCollider = nullptr;

        if (a->GetTag() == ColliderTag::EnemyAttack &&
            b->GetTag() == ColliderTag::Player)
        {
            enemyAttackCollider = a;
            playerCollider = b;
        }
        else if (a->GetTag() == ColliderTag::Player &&
            b->GetTag() == ColliderTag::EnemyAttack)
        {
            enemyAttackCollider = b;
            playerCollider = a;
        }

        if (enemyAttackCollider != nullptr && playerCollider != nullptr)
        {
            CharacterBase* enemy = enemyAttackCollider->GetOwner();

            if (enemy != nullptr && player != nullptr)
            {
                player->Damage(enemy->GetAttack());

                EnemyBase* enemyBase = dynamic_cast<EnemyBase*>(enemy);
                if (enemyBase != nullptr)
                {
                    enemyBase->DisableAttackCollider();
                }

                break;
            }
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
/// <summary>
/// リソース解放
/// </summary>
void CharacterManager::Release()
{
    collisionManager.Clear();

    if (player != nullptr)
    {
        player->Release();
        player.reset();
    }
    for (auto& enemy : enemies)
    {
        if (enemy != nullptr)
        {
            enemy->Release();
        }
    }

    enemies.clear();

    collisionManager.Clear();
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
/// <summary>
/// 
/// </summary>
void CharacterManager::ResolveEnemyCollision()
{
    const int resolveCount = 2;

    for (int count = 0; count < resolveCount; count++)
    {
        for (int i = 0; i < (int)enemies.size(); i++)
        {
            if (enemies[i] == nullptr || enemies[i]->IsDead())
            {
                continue;
            }

            CapsuleCollider* colA = enemies[i]->GetCapsuleCollider();
            if (colA == nullptr)
            {
                continue;
            }

            for (int j = i + 1; j < (int)enemies.size(); j++)
            {
                if (enemies[j] == nullptr || enemies[j]->IsDead())
                {
                    continue;
                }

                CapsuleCollider* colB = enemies[j]->GetCapsuleCollider();
                if (colB == nullptr)
                {
                    continue;
                }

                VECTOR posA = enemies[i]->GetPosition();
                VECTOR posB = enemies[j]->GetPosition();

                float radiusA = colA->GetRadius();
                float radiusB = colB->GetRadius();

                float dx = posB.x - posA.x;
                float dz = posB.z - posA.z;

                float distanceSq = dx * dx + dz * dz;
                float minDistance = radiusA + radiusB + 10.0f;

                if (distanceSq <= 0.0001f)
                {
                    dx = 1.0f;
                    dz = 0.0f;
                    distanceSq = 1.0f;
                }

                float distance = sqrtf(distanceSq);

                if (distance < minDistance)
                {
                    float overlap = minDistance - distance;

                    float nx = dx / distance;
                    float nz = dz / distance;

                    float push = overlap * 0.5f;

                    posA.x -= nx * push;
                    posA.z -= nz * push;

                    posB.x += nx * push;
                    posB.z += nz * push;

                    enemies[i]->SetPosition(posA);
                    enemies[j]->SetPosition(posB);

                    enemies[i]->UpdateCollider();
                    enemies[j]->UpdateCollider();
                }
            }
        }
    }
}
// 敵の生成関数
void CharacterManager::AddLittleEnemy(VECTOR pos)
{
    std::unique_ptr<EnemyBase> enemy = std::make_unique<LittleEnemy>();

    enemy->Init();
    enemy->SetPosition(pos);

    enemies.push_back(std::move(enemy));
}