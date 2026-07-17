#include "EnemyManager.h"
#include "LittleEnemy.h"
#include "Player.h"
#include <cmath>


EnemyManager::EnemyManager()
    : maxEnemyAttackCount(2)
{
}

EnemyManager::~EnemyManager()
{
    Release();
}

void EnemyManager::Init()
{
    enemies.clear();
    attackEnemies.clear();
}

void EnemyManager::Update(VECTOR playerPos)
{
    // 攻撃担当が死んでいたら、RemoveDeadEnemies前に外す
    for (auto& enemy : attackEnemies)
    {
        if (enemy != nullptr && enemy->IsDead() == true)
        {
            enemy = nullptr;
        }
    }

    RemoveDeadEnemies();

    // 無効な攻撃担当を整理
    CleanupAttackEnemies();

    // 攻撃担当が最大数に足りなければ追加
    while ((int)attackEnemies.size() < maxEnemyAttackCount)
    {
        EnemyBase* selectedEnemy = SelectAttackEnemy(playerPos);

        if (selectedEnemy == nullptr)
        {
            break;
        }

        attackEnemies.push_back(selectedEnemy);
    }

    // Enemy更新
    for (auto& enemy : enemies)
    {
        if (enemy == nullptr || enemy->IsDead() == true)
        {
            continue;
        }

        bool canAttack = IsAttackEnemy(enemy.get());

        enemy->Update(playerPos, canAttack);
    }

    ResolveEnemyCollision();
}
EnemyBase* EnemyManager::SelectAttackEnemy(VECTOR playerPos)
{
    EnemyBase* bestEnemy = nullptr;
    float bestScore = 999999999.0f;

    const float idealAttackStartDistance = 350.0f;

    for (auto& enemy : enemies)
    {
        if (enemy == nullptr || enemy->IsDead() == true)
        {
            continue;
        }
		// すでに攻撃担当のEnemyは選ばない
        if (IsAttackEnemy(enemy.get()) == true) {
			continue;
        }
        // 攻撃後クールタイム中のEnemyは選ばない
        if (enemy->CanAttack() == false)
        {
            continue;
        }

        VECTOR enemyPos = enemy->GetPosition();

        float dx = enemyPos.x - playerPos.x;
        float dz = enemyPos.z - playerPos.z;

        float distance = sqrtf(dx * dx + dz * dz);

        // 理想距離に近いEnemyほど選ばれやすい
        float score = fabsf(distance - idealAttackStartDistance);

        // ランダムを少し入れて毎回同じにならないようにする
        score += (float)GetRand(100);

        // 遠すぎるEnemyは少し不利
        if (distance > 700.0f)
        {
            score += 300.0f;
        }

        // 近すぎるEnemyは少し不利
        if (distance < 150.0f)
        {
            score += 150.0f;
        }

        if (score < bestScore)
        {
            bestScore = score;
            bestEnemy = enemy.get();
        }
    }

    return bestEnemy;
}
void EnemyManager::CleanupAttackEnemies()
{
    for (auto it = attackEnemies.begin(); it != attackEnemies.end();)
    {
        EnemyBase* enemy = *it;

        bool remove = false;

        if (enemy == nullptr)
        {
            remove = true;
        }
        else if (ExistsEnemy(enemy) == false)
        {
            remove = true;
        }
        else if (enemy->IsDead() == true)
        {
            remove = true;
        }
        else if (enemy->IsAttacking() == false && enemy->CanAttack() == false)
        {
            // 攻撃後の予約クールタイム中なら担当から外す
            remove = true;
        }

        if (remove == true)
        {
            it = attackEnemies.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

bool EnemyManager::ExistsEnemy(EnemyBase* enemy) const
{
    if (enemy == nullptr)
    {
        return false;
	}

    for (const auto& e : enemies)
    {
        if (e.get() == enemy)
        {
            return true;
        }
    }

    return false;
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
    attackEnemies.clear();
}
/// <summary>
/// 死亡した敵を削除する
/// </summary>
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
/// <summary>
/// 敵の数を取得する
/// </summary>
/// <returns>敵の数</returns>
int EnemyManager::GetEnemyCount() const
{
    return (int)enemies.size();
}

/// <summary>
/// プレイヤーの座標から最も近い敵を取得する
/// </summary>
/// <param name="playerPos"></param>
/// <param name="searchRange"></param>
/// <returns></returns>
EnemyBase* EnemyManager::GetNearestEnemy(VECTOR playerPos, float searchRange) const
{
    EnemyBase* nearestEnemy = nullptr;

    float nearestDistanceSq = searchRange * searchRange;

    for (const auto& enemy : enemies)
    {
        if (enemy == nullptr || enemy->IsDead() == true)
        {
            continue;
        }

        VECTOR enemyPos = enemy->GetPosition();

        float dx = enemyPos.x - playerPos.x;
        float dz = enemyPos.z - playerPos.z;

        float distanceSq = dx * dx + dz * dz;

        if (distanceSq < nearestDistanceSq)
        {
            nearestDistanceSq = distanceSq;
            nearestEnemy = enemy.get();
        }
    }

    return nearestEnemy;
}

/// <summary>
/// 
/// </summary>
/// <param name="player"></param>
void EnemyManager::ResolvePlayer(Player* player)
{
    if (player == nullptr)
    {
        return;
    }

    // 通常時はposition、攻撃中はHips位置
    VECTOR checkPos = player->GetCheckPosition();

    VECTOR correction = VGet(0.0f, 0.0f, 0.0f);

    const float playerRadius = 50.0f;
    const float enemyRadius = 50.0f;

    float minDistance = playerRadius + enemyRadius + 20.0f;

    // 攻撃中は体が前に出るので、少し広めに取る
    if (player->IsAttacking() == true)
    {
        minDistance = playerRadius + enemyRadius + 80.0f;
    }

    for (auto& enemy : enemies)
    {
        if (enemy == nullptr)
        {
            continue;
        }

        if (enemy->IsDead() == true)
        {
            continue;
        }

        VECTOR enemyPos = enemy->GetPosition();

        float dx = checkPos.x - enemyPos.x;
        float dz = checkPos.z - enemyPos.z;

        float distanceSq = dx * dx + dz * dz;

        if (distanceSq <= 0.0001f)
        {
            dx = 1.0f;
            dz = 0.0f;
            distanceSq = 1.0f;
        }

        float distance = sqrtf(distanceSq);

        if (distance >= minDistance)
        {
            continue;
        }

        float pushDistance = minDistance - distance;

        // 攻撃中は一気に押し戻すとカクつくので弱める
        if (player->IsAttacking() == true)
        {
            pushDistance *= 0.25f;
        }

        const float maxPushPerFrame = 5.0f;

        if (pushDistance > maxPushPerFrame)
        {
            pushDistance = maxPushPerFrame;
        }

        dx /= distance;
        dz /= distance;

        VECTOR push;
        push.x = dx * pushDistance;
        push.y = 0.0f;
        push.z = dz * pushDistance;

        correction = VAdd(correction, push);

        // 複数Enemy対策
        checkPos = VAdd(checkPos, push);
    }

    if (fabsf(correction.x) > 0.001f ||
        fabsf(correction.z) > 0.001f)
    {
        VECTOR newPlayerPos = VAdd(
            player->GetPosition(),
            correction
        );

        player->SetPositionForCollision(newPlayerPos);
    }
}
bool EnemyManager::ContainsEnemy(EnemyBase* enemy) const
{
    return ExistsEnemy(enemy);
}
/// <summary>
/// 敵のコライダーをCollisionManagerに追加する
/// </summary>
/// <param name="collisionManager"></param>
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

/// <summary>
/// 最大同時攻撃可能数を設定する
/// </summary>
/// <param name="count"></param>
void EnemyManager::SetMaxEnemyAttackCount(int count)
{
    if (count < 1)
        count = 1;

    maxEnemyAttackCount = count;

	// 攻撃担当のEnemyが最大数を超えていたら整理する
    while ((int)attackEnemies.size() > maxEnemyAttackCount)
    {
        attackEnemies.pop_back();
    }
}

/// <summary>
/// すべての敵が死亡しているかどうかを判定する
/// </summary>
/// <returns></returns>
bool EnemyManager::IsAllEnemyDead() const
{
    return enemies.empty();
}

/// <summary>
/// 指定されたEnemyが攻撃担当かどうかを判定する
/// </summary>
/// <param name="enemy">判定するEnemy</param>
/// <returns>攻撃担当であればtrue、それ以外はfalse</returns>
bool EnemyManager::IsAttackEnemy(EnemyBase* enemy) const
{
    if (enemy == nullptr)
    {
        return false;
    }

    for (EnemyBase* attackEnemy : attackEnemies)
    {
        if (attackEnemy == enemy)
        {
            return true;
        }
    }

    return false;
}
/// <summary>
/// 指定されたウェーブ番号に応じて敵を出現させる
/// </summary>
/// <param name="waveNo">ウェーブ番号</param>
void EnemyManager::SpawnWave(int waveNo)
{
    ClearEnemies();
	// ウェーブ番号に応じて敵を出現させる
    if (waveNo == 1)
    {
		//エネミーの最大同時攻撃可能数
        SetMaxEnemyAttackCount(1);
        AddLittleEnemy(VGet(300.0f, 0.0f, 300.0f));
        AddLittleEnemy(VGet(-300.0f, 0.0f, 300.0f));
    }
    else if (waveNo == 2)
    {
        SetMaxEnemyAttackCount(2);
        AddLittleEnemy(VGet(300.0f, 0.0f, 300.0f));
        AddLittleEnemy(VGet(-300.0f, 0.0f, 300.0f));
        AddLittleEnemy(VGet(0.0f, 0.0f, -400.0f));
    }
    else if (waveNo == 3)
    {
        SetMaxEnemyAttackCount(3);
        AddLittleEnemy(VGet(300.0f, 0.0f, 300.0f));
        AddLittleEnemy(VGet(-300.0f, 0.0f, 300.0f));
        AddLittleEnemy(VGet(300.0f, 0.0f, -300.0f));
        AddLittleEnemy(VGet(-300.0f, 0.0f, -300.0f));
    }
}

/// <summary>
/// 敵同士の衝突を解決する
/// </summary>
void EnemyManager::ResolveEnemyCollision()
{
    const int resolveCount = 2;
	// resolveCount回繰り返して衝突を解決する
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

void EnemyManager::ResolveStageCollision(const StageManager& stageManager)
{
    for (auto& enemy : enemies)
    {
        if (enemy == nullptr)
        {
            continue;
        }

        if (enemy->IsDead() == true)
        {
            continue;
        }

        VECTOR fixedPos = stageManager.ClampPosition(
            enemy->GetPosition(),
            50.0f
        );
        fixedPos = stageManager.FitPositionToGround(fixedPos, 0.0f);
        
        enemy->SetPositionForCollision(fixedPos);
    }
}
