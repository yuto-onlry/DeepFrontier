#include "PlayerCombo.h"

PlayerCombo::PlayerCombo()
    : comboIndex(0),
    comboRequest(false)
{
}

void PlayerCombo::Init()
{
    comboIndex = 0;
    comboRequest = false;
}

void PlayerCombo::Reset()
{
    comboIndex = 0;
    comboRequest = false;
}

void PlayerCombo::SetIndex(int index)
{
    comboIndex = index;
    comboRequest = false;
}

int PlayerCombo::GetIndex() const
{
    return comboIndex;
}

void PlayerCombo::RequestNext()
{
    comboRequest = true;
}

bool PlayerCombo::CanNext() const
{
    return comboIndex < MaxCombo - 1;
}

bool PlayerCombo::ShouldNext() const
{
    return comboRequest == true && CanNext() == true;
}

int PlayerCombo::GetNextIndex() const
{
    return comboIndex + 1;
}

AnimationType PlayerCombo::GetAnimationType(bool isLockOn) const
{
    if (isLockOn == true)
    {
        if (comboIndex == 0)
        {
            return AnimationType::LockOnAttack;
        }

        if (comboIndex == 1)
        {
            return AnimationType::StrongAttack;
        }

        return AnimationType::Attack;
    }

    if (comboIndex == 0)
    {
        return AnimationType::Attack;
    }

    if (comboIndex == 1)
    {
        return AnimationType::StrongAttack;
    }

    return AnimationType::Attack;
}

PlayerAction::AttackData PlayerCombo::GetAttackData() const
{
    PlayerAction::AttackData data;

    if (comboIndex == 0)
    {
        data.actionTime = 60;
        data.hitStartFrame = 14;
        data.hitEndFrame = 25;
        return data;
    }

    if (comboIndex == 1)
    {
        data.actionTime = 55;
        data.hitStartFrame = 12;
        data.hitEndFrame = 24;
        return data;
    }

    data.actionTime = 70;
    data.hitStartFrame = 18;
    data.hitEndFrame = 35;
    return data;
}