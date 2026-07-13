#include "PlayerAction.h"
#include "CharacterBase.h"

PlayerAction::PlayerAction()
    : currentAction(PlayerActionType::None),
    actionTimer(0),
    attackFrame(0),
    attackHitStartFrame(0),
    attackHitEndFrame(0),
    attackHit(false)
{
}

PlayerAction::~PlayerAction()
{
}

void PlayerAction::Init(CharacterBase* owner)
{
    currentAction = PlayerActionType::None;
    actionTimer = 0;

    attackFrame = 0;
    attackHitStartFrame = 10;
    attackHitEndFrame = 16;

    attackHit = false;

    attackCollider.SetTag(ColliderTag::PlayerAttack);
    attackCollider.SetOwner(owner);
    attackCollider.SetRadius(80.0f);
    attackCollider.SetActive(false);
}

void PlayerAction::StartAttack(VECTOR playerPos, VECTOR forward)
{
    AttackData attackData;

    attackData.actionTime = 60;
    attackData.hitStartFrame = 14;
    attackData.hitEndFrame = 25;

    StartAttack(playerPos, forward, attackData);
}

void PlayerAction::StartAttack(
    VECTOR playerPos,
    VECTOR forward,
    const AttackData& attackData
)
{
    currentAction = PlayerActionType::Attack;

    actionTimer = attackData.actionTime;

    // 攻撃のフレーム数をリセット
    attackFrame = 0;

    // 判定を出すフレーム
    attackHitStartFrame = attackData.hitStartFrame;

    // 判定を消すフレーム
    attackHitEndFrame = attackData.hitEndFrame;

    attackHit = false;

    attackCollider.SetActive(false);
}

void PlayerAction::StartAvoid()
{
    currentAction = PlayerActionType::Avoid;
    actionTimer = 20;

    attackCollider.SetActive(false);
}

void PlayerAction::Update(VECTOR playerPos, VECTOR forward)
{
    if (actionTimer <= 0)
    {
        currentAction = PlayerActionType::None;
        attackCollider.SetActive(false);
        return;
    }

    actionTimer--;

    if (currentAction == PlayerActionType::Attack)
    {
        attackFrame++;

        // 攻撃判定を出すフレームだけtrue
        if (attackFrame >= attackHitStartFrame &&
            attackFrame <= attackHitEndFrame &&
            attackHit == true)
        {
            attackCollider.SetActive(true);

            attackCollider.SetPosition(
                VGet(
                    playerPos.x + forward.x * 120.0f,
                    playerPos.y + 120.0f,
                    playerPos.z + forward.z * 120.0f
                )
            );
        }
        else
        {
            attackCollider.SetActive(false);
        }
    }
    else if (currentAction == PlayerActionType::Avoid)
    {
        attackCollider.SetActive(false);
    }

    if (actionTimer <= 0)
    {
        currentAction = PlayerActionType::None;
        attackCollider.SetActive(false);
        attackHit = false;
        attackFrame = 0;
    }
}

bool PlayerAction::IsAction() const
{
    return actionTimer > 0;
}

bool PlayerAction::IsAttack() const
{
    return currentAction == PlayerActionType::Attack;
}

bool PlayerAction::IsComboAcceptFrame() const
{
    if (currentAction != PlayerActionType::Attack)
    {
        return false;
    }

    // 連続攻撃の入力受付時間
    // まずは攻撃開始から15F以降なら予約できる形にする
    if (attackFrame >= 15)
    {
        return true;
    }

    return false;
}

void PlayerAction::AttackCollider()
{
    attackCollider.SetActive(false);
    attackHit = true;
}

SphereCollider* PlayerAction::GetAttackCollider()
{
    return &attackCollider;
}
