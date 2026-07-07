#include "AnimationPreset.h"

void AnimationPreset::SetAnimationPlayer(AnimationManager& animManager, int modelHandle)
{
    animManager.Init(modelHandle);

    if (animManager.LoadAnimModel("../Animation/Player/PlayerAnimation.mv1") == true ||
		animManager.LoadAnimModel("../Animation/Player/PlayerAnimation2.mv1") == true)
    {
		// アニメーション指定+割り振り
        animManager.SetAnimIndex(AnimationType::Idle, 9);
		animManager.SetAnimIndex(AnimationType::Walk, 42);
        animManager.SetAnimIndex(AnimationType::Run, 36);
		animManager.SetAnimIndex(AnimationType::Attack, 39);
		animManager.SetAnimIndex(AnimationType::StrongAttack, 12);
		animManager.SetAnimIndex(AnimationType::AttackLockOn, 40);
		animManager.SetAnimIndex(AnimationType::JumpStart, 16);
		animManager.SetAnimIndex(AnimationType::JumpLoop, 15);
		animManager.SetAnimIndex(AnimationType::JumpEnd, 14);
		animManager.SetAnimIndex(AnimationType::Avoid, 27);
		animManager.SetAnimIndex(AnimationType::Damage, 1);
		animManager.SetAnimIndex(AnimationType::GetUp, 16);
		animManager.SetAnimIndex(AnimationType::Dead, 8);

        animManager.ChangeAnim(AnimationType::Idle);
    }
}

void AnimationPreset::SetAnimationLittleEnemy(AnimationManager& animManager, int modelHandle)
{
    animManager.Init(modelHandle);
	// 敵のアニメーションを読み込む
    animManager.LoadAnimModelForType(
        AnimationType::Idle,
        "../Animation/Enemy/Idle.mv1",
        0
    );
    animManager.LoadAnimModelForType(
        AnimationType::Run,
        "../Animation/Enemy/Ran.mv1",
        0
    );
    animManager.LoadAnimModelForType(
        AnimationType::Attack,
        "../Animation/Enemy/Attack.mv1",
        0
    );
    animManager.LoadAnimModelForType(
        AnimationType::LateralMove,
        "../Animation/Enemy/LateralMove.mv1",
        0
    );


    animManager.ChangeAnim(AnimationType::Idle);
}
