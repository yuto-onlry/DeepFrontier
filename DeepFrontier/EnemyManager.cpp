#include "EnemyManager.h"
#include "LittleEnemy.h"

EnemyManager::EnemyManager()
    : maxEnemyAttackCount(1)
{
}

EnemyManager::~EnemyManager()
{
    Release();
}

void EnemyManager::Init()
{
}

void EnemyManager::Update(VECTOR playerPos)
{
    RemoveDeadEnemies();

    int currentAttackCount = 0;

    // すでに攻撃中の敵を数える
    for (auto& enemy : enemies)
    {
        if (enemy != nullptr && enemy->IsDead() == false)
        {
            if (enemy->IsAttacking() == true)
            {
                currentAttackCount++;
            }
        }
    }

    // 敵更新
    for (auto& enemy : enemies)
    {
        if (enemy == nullptr || enemy->IsDead() == true)
        {
            continue;
        }

        bool wasAttacking = enemy->IsAttacking();

        bool canAttack = false;

        // 攻撃中の敵はそのまま攻撃継続
        if (wasAttacking == true)
        {
            canAttack = true;
        }
        // 攻撃中の敵が上限未満なら攻撃許可
        else if (currentAttackCount < maxEnemyAttackCount)
        {
            canAttack = true;
        }

        enemy->Update(playerPos, canAttack);

        // このUpdateで攻撃を開始した場合
        if (wasAttacking == false && enemy->IsAttacking() == true)
        {
            currentAttackCount++;
        }
    }

    // 敵同士の重なり解消
    ResolveEnemyCollision();
}

void EnemyManager::Draw()
{
    for (auto& enemy : enemies)
    {
        if (enemy != nullptr)
        {
            enemy->Draw();
        }
    }
}

void EnemyManager::Release()
{
    ClearEnemies();
}

void EnemyManager::AddLittleEnemy(VECTOR pos)
{
    std::unique_ptr<EnemyBase> enemy = std::make_unique<LittleEnemy>();

    enemy->Init();
    enemy->SetPosition(pos);

    enemies.push_back(std::move(enemy));
}

void EnemyManager::ClearEnemies()
{
    for (auto& enemy : enemies)
    {
        if (enemy != nullptr)
        {
            enemy->Release();
        }
    }

    enemies.clear();
}

void EnemyManager::RemoveDeadEnemies()
{
    for (auto it = enemies.begin(); it != enemies.end();)
    {
        if ((*it) != nullptr && (*it)->IsDead() == true)
        {
            (*it)->Release();
            it = enemies.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

bool EnemyManager::IsAllEnemyDead() const
{
    return enemies.empty();
}

int EnemyManager::GetEnemyCount() const
{
    return (int)enemies.size();
}

void EnemyManager::AddColliders(CollisionManager& collisionManager)
{
    for (auto& enemy : enemies)
    {
        if (enemy != nullptr && enemy->IsDead() == false)
        {
            collisionManager.AddCollider(enemy->GetCapsuleCollider());
            collisionManager.AddCollider(enemy->GetAttackCollider());
        }
    }
}

void EnemyManager::SpawnWave(int waveNo)
{
    ClearEnemies();

    if (waveNo == 1)
    {
        AddLittleEnemy(VGet(300.0f, 0.0f, 300.0f));
        AddLittleEnemy(VGet(-300.0f, 0.0f, 300.0f));
    }
    else if (waveNo == 2)
    {
        AddLittleEnemy(VGet(300.0f, 0.0f, 300.0f));
        AddLittleEnemy(VGet(-300.0f, 0.0f, 300.0f));
        AddLittleEnemy(VGet(0.0f, 0.0f, -400.0f));
    }
    else if (waveNo == 3)
    {
        AddLittleEnemy(VGet(300.0f, 0.0f, 300.0f));
        AddLittleEnemy(VGet(-300.0f, 0.0f, 300.0f));
        AddLittleEnemy(VGet(300.0f, 0.0f, -300.0f));
        AddLittleEnemy(VGet(-300.0f, 0.0f, -300.0f));
    }
}

void EnemyManager::ResolveEnemyCollision()
{
    const int resolveCount = 2;

    for (int count = 0; count < resolveCount; count++)
    {
        for (int i = 0; i < (int)enemies.size(); i++)
        {
            if (enemies[i] == nullptr || enemies[i]->IsDead() == true)
            {
                continue;
            }

            for (int j = i + 1; j < (int)enemies.size(); j++)
            {
                if (enemies[j] == nullptr || enemies[j]->IsDead() == true)
                {
                    continue;
                }

                VECTOR posA = enemies[i]->GetPosition();
                VECTOR posB = enemies[j]->GetPosition();

                float radiusA = enemies[i]->GetCapsuleCollider()->GetRadius();
                float radiusB = enemies[j]->GetCapsuleCollider()->GetRadius();

                float dx = posB.x - posA.x;
                float dz = posB.z - posA.z;

                float distanceSq = dx * dx + dz * dz;

                if (distanceSq <= 0.0001f)
                {
                    dx = 1.0f;
                    dz = 0.0f;
                    distanceSq = 1.0f;
                }

                float distance = sqrtf(distanceSq);

                float minDistance = radiusA + radiusB + 10.0f;

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