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

    /// 現在のコンボ状態・ロックオン状態に応じたアニメーション種別を返す
    AnimationType GetAnimationType(bool isLockOn) const;
    // 現在のコンボ段階に対応する攻撃データを取得する
    PlayerAction::AttackData GetAttackData() const;
};