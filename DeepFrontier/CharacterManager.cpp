#include "CharacterManager.h"

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

    enemyManager.Init();
    lockOnManager.Init();
}

void CharacterManager::Update(const InputManager& inputManager,VECTOR cameraForward,VECTOR cameraRight)
{
    if (player != nullptr)
    {
        player->Update(inputManager, cameraForward, cameraRight);
    }

    VECTOR playerPos = GetPlayerPosition();

    // 敵全体の更新は EnemyManager に任せる
    enemyManager.Update(playerPos);

    // ロックオン更新
    if (player != nullptr)
    {
        lockOnManager.Update(inputManager, player->GetPosition(), enemyManager);
    }

    // コライダー登録
    collisionManager.Clear();

    if (player != nullptr)
    {
        collisionManager.AddCollider(player->GetCapsuleCollider());

        for (int i = 0; i < player->GetAttackColliderCount(); i++)
        {
            collisionManager.AddCollider(player->GetAttackCollider(i));
        }
    }

    enemyManager.AddColliders(collisionManager);

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

        // プレイヤー攻撃 × 敵
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

            if (enemy != nullptr && player != nullptr)
            {
                enemy->Damage(player->GetAttack());
                player->DisableAttackCollider();
                break;
            }
        }

        // 敵攻撃 × プレイヤー
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
    {
        player->Draw();
    }

    enemyManager.Draw();
	lockOnManager.Draw();
    collisionManager.DrawDebug();
}

void CharacterManager::Release()
{
    collisionManager.Clear();
	lockOnManager.Clear();

    if (player != nullptr)
    {
        player->Release();
        player.reset();
    }

    enemyManager.Release();

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

bool CharacterManager::IsAllEnemyDead() const
{
    return enemyManager.IsAllEnemyDead();
}

int CharacterManager::GetEnemyCount() const
{
    return enemyManager.GetEnemyCount();
}

void CharacterManager::SpawnWave(int waveNo)
{
    lockOnManager.Clear();
    enemyManager.SpawnWave(waveNo);
}