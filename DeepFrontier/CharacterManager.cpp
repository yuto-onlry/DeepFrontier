#include "CharacterManager.h"

namespace
{
    const int FOCUS_HIT_REQUIRED = 2;   // 同じ敵に2回当てたらFocus
    const int FOCUS_HIT_KEEP_TIME = 90; // 90フレーム以内なら連続ヒット扱い
}

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
    battlePhase = BattlePhase::Field;

    focusCandidateEnemy = nullptr;
    focusEnemy = nullptr;

    focusHitCount = 0;
    focusHitTimer = 0;
}

void CharacterManager::Update(
    const InputManager& inputManager,
    VECTOR cameraForward,
    VECTOR cameraRight
)
{
    // 先にロックオン更新
    if (player != nullptr)
    {
        lockOnManager.Update(
            inputManager,
            player->GetPosition(),
            enemyManager
        );
    }
    // Focusフェーズ更新
    FocusPhase();

    // ロックオン情報をPlayerへ渡す
    if (player != nullptr)
    {
        player->Update(
            inputManager,
            cameraForward,
            cameraRight,
            lockOnManager.IsLockOn(),
            lockOnManager.GetTargetPosition()
        );
    }

    VECTOR playerPos = GetPlayerPosition();

    // 敵全体の更新
    enemyManager.Update(playerPos);

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
                EnemyBase* hitEnemy = dynamic_cast<EnemyBase*>(enemy);

                enemy->Damage(player->GetAttack());
                player->DisableAttackCollider();

                // 敵が生きている場合だけFocus用ヒットとして数える
                if (hitEnemy != nullptr && hitEnemy->IsDead() == false)
                {
                    RegisterFocusHit(hitEnemy);
                }

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

void CharacterManager::FocusPhase()
{
    // Focus候補のヒットカウント時間
    if (focusHitTimer > 0)
    {
        focusHitTimer--;
    }
    else
    {
        focusHitCount = 0;
        focusCandidateEnemy = nullptr;
    }

    // Focus中の対象が消えた・死んだら解除
    if (battlePhase == BattlePhase::Focus)
    {
        if (focusEnemy == nullptr ||
            enemyManager.ContainsEnemy(focusEnemy) == false ||
            focusEnemy->IsDead() == true)
        {
            EndFocus();
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

VECTOR CharacterManager::GetLockOnTargetPosition() const
{
    return lockOnManager.GetTargetPosition();
}

Player* CharacterManager::GetPlayer()
{
    return player.get();
}
void CharacterManager::RegisterFocusHit(EnemyBase* hitEnemy)
{
    if (hitEnemy == nullptr)
    {
        return;
    }

    // すでにFocus中なら、新しくカウントしない
    if (battlePhase == BattlePhase::Focus)
    {
        return;
    }

    // 同じ敵ならカウント継続
    if (focusCandidateEnemy == hitEnemy)
    {
        focusHitCount++;
    }
    else
    {
        focusCandidateEnemy = hitEnemy;
        focusHitCount = 1;
    }

    focusHitTimer = FOCUS_HIT_KEEP_TIME;

    if (focusHitCount >= FOCUS_HIT_REQUIRED)
    {
        StartFocus(hitEnemy);
    }
}
void CharacterManager::StartFocus(EnemyBase* enemy)
{
    if (enemy == nullptr)
    {
        return;
    }

    battlePhase = BattlePhase::Focus;

    focusEnemy = enemy;

    focusCandidateEnemy = nullptr;
    focusHitCount = 0;
    focusHitTimer = 0;

    // Focus対象を強制ロックオン
    lockOnManager.LockOn(enemy);
}

void CharacterManager::EndFocus()
{
    battlePhase = BattlePhase::Field;

    focusEnemy = nullptr;
    focusCandidateEnemy = nullptr;

    focusHitCount = 0;
    focusHitTimer = 0;

    lockOnManager.Clear();
}

bool CharacterManager::IsFocusPhase() const
{
    return battlePhase == BattlePhase::Focus;
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
bool CharacterManager::IsLockOn() const
{
    return lockOnManager.IsLockOn();
}

EnemyBase* CharacterManager::GetLockOnTarget() const
{
    return lockOnManager.GetTargetEnemy();
}