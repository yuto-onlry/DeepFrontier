#include "AnimationPreset.h"

void AnimationPreset::SetAnimationPlayer(AnimationManager& animManager, int modelHandle)
{
    animManager.Init(modelHandle);

    if (animManager.LoadAnimModel("../Animation/Player/PlayerAnimation.mv1") == true ||
		animManager.LoadAnimModel(".../Animation/Player/PlayerAnimation2.mv1") == true)
    {
		// アニメーション番号を指定+割り振り
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
  //  if (animManager.LoadAnimModel("../Animation/Enemy/LittleEnemyAnimation.mv1") == true)
  //  {
		//// アニメーション番号を指定+割り振り
  //      animManager.SetAnimIndex(AnimationType::Idle, 0);
  //      animManager.SetAnimIndex(AnimationType::Run, 1);
  //      animManager.SetAnimIndex(AnimationType::Attack, 2);
  //      animManager.ChangeAnim(AnimationType::Idle);
  //  }

}