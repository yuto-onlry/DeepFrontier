#include "PlayerAction.h"
#include "CharacterBase.h"

PlayerAction::PlayerAction()
    : currentAction(PlayerActionType::None),
    actionTimer(0),
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
    attackHit = false;

    attackCollider.SetTag(ColliderTag::PlayerAttack);
    attackCollider.SetOwner(owner);
    attackCollider.SetRadius(80.0f);
    attackCollider.SetActive(false);
}

void PlayerAction::StartAttack(VECTOR playerPos, VECTOR forward)
{
    currentAction = PlayerActionType::Attack;
    actionTimer = 30;
    attackHit = false;

    attackCollider.SetActive(true);
    attackCollider.SetPosition(
    VGet(playerPos.x + forward.x * 120.0f,playerPos.y + 120.0f,playerPos.z + forward.z * 120.0f));
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

    if (currentAction == PlayerActionType::Attack && attackHit == false)
    {
        attackCollider.SetActive(true);
        attackCollider.SetPosition(
        VGet(playerPos.x + forward.x * 120.0f,playerPos.y + 120.0f,playerPos.z + forward.z * 120.0f));
    }
    else
        attackCollider.SetActive(false);

    if (actionTimer <= 0)
    {
        currentAction = PlayerActionType::None;
        attackCollider.SetActive(false);
        attackHit = false;
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

void PlayerAction::AttackCollider()
{
    attackCollider.SetActive(false);
    attackHit = true;
}

SphereCollider* PlayerAction::GetAttackCollider()
{
    return &attackCollider;
}