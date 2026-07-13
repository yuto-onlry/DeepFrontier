#pragma once

#include "animationManager.h"
#include "PlayerAction.h"

class PlayerCombo
{
private:
    int comboIndex;
    bool comboRequest;
	// 最大コンボ数
    static const int MaxCombo = 3;

public:
    PlayerCombo();

    void Init();
    void Reset();

    void SetIndex(int index);
    int GetIndex() const;

    void RequestNext();
    bool ShouldNext() const;
    bool CanNext() const;
    int GetNextIndex() const;

    AnimationType GetAnimationType(bool isLockOn) const;
    PlayerAction::AttackData GetAttackData() const;
};