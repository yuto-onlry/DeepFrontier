#include "AnimationPreset.h"

void AnimationPreset::SetAnimationPlayer(AnimationManager& animManager, int modelHandle)
{
    animManager.Init(modelHandle);

    animManager.LoadAnimModelForType(
        AnimationType::Idle,
        "../Animation/Player/Idle.mv1",
        0
    );

    animManager.LoadAnimModelForType(
        AnimationType::Walk,
        "../Animation/Player/Walk.mv1",
        0
    );

    animManager.LoadAnimModelForType(
        AnimationType::WalkBack,
        "../Animation/Player/WalkBack.mv1",
        0
    );

    animManager.LoadAnimModelForType(
        AnimationType::Run,
        "../Animation/Player/Run.mv1",
        0
    );

    animManager.LoadAnimModelForType(
        AnimationType::Attack,
        "../Animation/Player/Attack2.mv1",
        0
    );

    animManager.LoadAnimModelForType(
        AnimationType::Avoid,
        "../Animation/Player/Avoid.mv1",
        0
    );

    animManager.LoadAnimModelForType(
        AnimationType::Jump,
        "../Animation/Player/Jump.mv1",
        0
    );

    animManager.LoadAnimModelForType(
        AnimationType::Damage,
        "../Animation/Player/Guard.mv1",
        0
    );

    animManager.LoadAnimModelForType(
        AnimationType::GetUp,
        "../Animation/Player/GetUp.mv1",
        0
    );

    animManager.LoadAnimModelForType(
        AnimationType::Dead,
        "../Animation/Player/Dead.mv1",
        0
    );

    animManager.LoadAnimModelForType(
        AnimationType::StrongAttack,
        "../Animation/Player/Attack.mv1",
        0
    );
    animManager.LoadAnimModelForType(
        AnimationType::StrongAttack2,
        "../Animation/Player/StrongAttack2.mv1",
        0
    );


    animManager.LoadAnimModelForType(
        AnimationType::LockOnIdle,
        "../Animation/Player/LockOnIdle.mv1",
        0
    );
    animManager.LoadAnimModelForType(
        AnimationType::LockOnWalk,
        "../Animation/Player/LockOnWalk.mv1",
        0
    );
    animManager.LoadAnimModelForType(
        AnimationType::LockOnLateralMove,
        "../Animation/Player/LockOnLateralMove.mv1",
        0
    );

    animManager.ChangeAnim(AnimationType::Idle);
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
        AnimationType::MoveRight,
        "../Animation/Enemy/MoveRight.mv1",
        0
    );
    animManager.LoadAnimModelForType(
        AnimationType::MoveLeft,
        "../Animation/Enemy/MoveLeft.mv1",
        0
    );
    animManager.LoadAnimModelForType(
        AnimationType::KnockbackDown,
        "../Animation/Enemy/KnockbackDown.mv1",
        0
    );
    animManager.LoadAnimModelForType(
        AnimationType::GetUp,
        "../Animation/Enemy/GetUp.mv1",
        0
    );
    animManager.LoadAnimModelForType(
        AnimationType::Damage,
        "../Animation/Enemy/Damage.mv1",
        0
    );
    animManager.ChangeAnim(AnimationType::Idle);
}
